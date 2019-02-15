#include "stdafx.h"
#include "ParserTable.h"
#include "ParserState.h"
#include "../Grammar/GrammarUtils.h"

using namespace grammarlib;

namespace
{
std::set<std::string> GatherAcceptablesAndFollowingsIfHasEmptiness(
	const IGrammar& grammar,
	const std::string& nonterminal
)
{
	auto acceptables = GatherBeginningSymbolsOfNonterminal(grammar, nonterminal);
	if (NonterminalHasEmptiness(grammar, nonterminal))
	{
		auto followings = GatherFollowingSymbols(grammar, nonterminal);
		acceptables.insert(followings.begin(), followings.end());
	}
	return acceptables;
}

class ParserStateFactory
{
public:
	ParserStateFactory(const IGrammar& grammar, const IParserTable& table)
		: m_grammar(grammar)
		, m_table(table)
	{
	}

	std::unique_ptr<ParserState> OnTerminal(const IGrammarSymbol& symbol, size_t row, size_t col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Terminal);
		auto state = std::make_unique<ParserState>();

		state->SetFlag(StateFlag::Error, true);
		state->SetFlag(StateFlag::Shift, !symbol.HasAttribute());
		state->SetFlag(StateFlag::End, symbol.GetName() == m_grammar.GetEndTerminal() && !symbol.HasAttribute());

		using boost::make_optional;
		const IGrammarProduction& production = m_grammar.GetProduction(row);

		state->SetNextAddress(
			production.IsLastSymbol(col) && !symbol.HasAttribute() ?
			boost::none : make_optional(m_table.GetStatesCount() + 1)
		);

		state->SetAcceptableTerminals({ symbol.GetName() });
		state->SetName(symbol.GetName());

		return state;
	}

	std::unique_ptr<ParserState> OnNonterminal(const IGrammarSymbol& symbol, size_t row, size_t col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Nonterminal);
		auto state = std::make_unique<ParserState>();

		state->SetFlag(StateFlag::Error, true);

		const IGrammarProduction& production = m_grammar.GetProduction(row);
		state->SetFlag(StateFlag::Push, !production.IsLastSymbol(col) || symbol.HasAttribute());

		state->SetNextAddress(GetProductionIndex(m_grammar, symbol.GetName()));
		state->SetAcceptableTerminals(GatherAcceptablesAndFollowingsIfHasEmptiness(m_grammar, symbol.GetName()));
		state->SetName(symbol.GetName());

		return state;
	}

	std::unique_ptr<ParserState> OnEpsilon(const IGrammarSymbol& symbol, size_t row, size_t col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Epsilon);
		auto state = std::make_unique<ParserState>();

		using boost::make_optional;
		state->SetNextAddress(
			symbol.HasAttribute() ?
			make_optional(m_table.GetStatesCount() + 1) : boost::none
		);

		state->SetAcceptableTerminals(GatherBeginningSymbolsOfProduction(m_grammar, int(row)));
		state->SetFlag(StateFlag::Error, true);
		state->SetName(symbol.GetName());

		return state;
	}

	std::unique_ptr<ParserState> OnAttribute(const IGrammarSymbol& symbol, size_t row, size_t col)
	{
		assert(symbol.HasAttribute());
		auto state = std::make_unique<ParserState>();

		state->SetFlag(StateFlag::Attribute, true);
		state->SetFlag(StateFlag::Shift, symbol.GetType() == GrammarSymbolType::Terminal);
		state->SetFlag(StateFlag::End, symbol.GetName() == m_grammar.GetEndTerminal());

		using boost::make_optional;
		const IGrammarProduction& production = m_grammar.GetProduction(row);
		state->SetNextAddress(
			production.IsLastSymbol(col) ?
			boost::none : make_optional(m_table.GetStatesCount() + 1)
		);

		state->SetName(*symbol.GetAttribute());
		return state;
	}

private:
	const IGrammar& m_grammar;
	const IParserTable& m_table;
};
}

void ParserTable::AddState(std::unique_ptr<IParserState> && state)
{
	m_states.push_back(std::move(state));
}

IParserState& ParserTable::GetState(size_t index)
{
	if (index >= m_states.size())
	{
		throw std::out_of_range("index must be less than states count");
	}
	return *m_states[index];
}

const IParserState& ParserTable::GetState(size_t index) const
{
	if (index >= m_states.size())
	{
		throw std::out_of_range("index must be less than states count");
	}
	return *m_states[index];
}

size_t ParserTable::GetStatesCount() const
{
	return m_states.size();
}

std::unique_ptr<ParserTable> ParserTable::Create(const grammarlib::IGrammar& grammar)
{
	auto table = std::make_unique<ParserTable>();
	auto factory = std::make_unique<ParserStateFactory>(grammar, *table);

	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		auto state = std::make_unique<ParserState>();
		state->SetFlag(StateFlag::Error, !ProductionHasAlternativeWithHigherIndex(grammar, i));
		state->SetName(grammar.GetProduction(i).GetLeftPart());
		state->SetNextAddress(boost::none); // Address will be reassigned later
		state->SetAcceptableTerminals(GatherBeginningSymbolsOfProduction(grammar, int(i)));
		table->AddState(std::move(state));
	}

	for (size_t row = 0; row < grammar.GetProductionsCount(); ++row)
	{
		const grammarlib::IGrammarProduction& production = grammar.GetProduction(row);
		unsigned attributesCount = 0;

		for (size_t col = 0; col < production.GetSymbolsCount(); ++col)
		{
			const IGrammarSymbol& symbol = production.GetSymbol(col);

			switch (symbol.GetType())
			{
			case GrammarSymbolType::Terminal:
				table->AddState(factory->OnTerminal(symbol, row, col));
				break;
			case GrammarSymbolType::Nonterminal:
				table->AddState(factory->OnNonterminal(symbol, row, col));
				break;
			case GrammarSymbolType::Epsilon:
				table->AddState(factory->OnEpsilon(symbol, row, col));
				break;
			}

			if (auto attribute = symbol.GetAttribute())
			{
				table->AddState(factory->OnAttribute(symbol, row, col));
				++attributesCount;
			}
		}

		// Reassigning address that we skipped on first loop
		table->GetState(row).SetNextAddress(table->GetStatesCount() - production.GetSymbolsCount() - attributesCount);
	}

	return table;
}
