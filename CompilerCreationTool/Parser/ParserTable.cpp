#include "stdafx.h"
#include "ParserTable.h"
#include "ParserState.h"
#include "../Grammar/GrammarUtils.h"

namespace
{
std::set<std::string> GatherAcceptablesAndFollowingsIfHasEmptiness(
	const grammarlib::IGrammar& grammar,
	const std::string& nonterminal
)
{
	auto acceptables = grammarlib::GatherBeginningSymbolsOfNonterminal(grammar, nonterminal);
	if (grammarlib::NonterminalHasEmptiness(grammar, nonterminal))
	{
		auto followings = grammarlib::GatherFollowingSymbols(grammar, nonterminal);
		acceptables.insert(followings.begin(), followings.end());
	}
	return acceptables;
}

class ParserStateFactory
{
public:
	ParserStateFactory(IGrammar& grammar, IParserTable& table)
		: m_grammar(grammar)
		, m_table(table)
	{
	}

	std::unique_ptr<ParserState> CreateState(const IGrammarSymbol& symbol, int row, int col)
	{
		switch (symbol.GetType())
		{
		case GrammarSymbolType::Terminal:
			return OnTerminal(symbol, row, col);
		case GrammarSymbolType::Nonterminal:
			return OnNonterminal(symbol, row, col);
		case GrammarSymbolType::Epsilon:
			return OnEpsilon(symbol, row, col);
		}
		throw std::logic_error("can't create parser state because of undefined grammar symbol type");
	}

private:
	std::unique_ptr<ParserState> OnTerminal(const IGrammarSymbol& symbol, int row, int col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Terminal);
		auto state = std::make_unique<ParserState>();

		int flags = StateFlags::Error;
		if (!symbol.HasAttribute())
		{
			flags |= StateFlags::Shift;
		}
		if (symbol.GetName() == m_grammar.GetEndTerminal() && !symbol.HasAttribute())
		{
			flags |= StateFlags::End;
		}
		state->SetFlags(flags);

		const IGrammarProduction& production = m_grammar.GetProduction(row);
		if (production.IsLastSymbol(col) && !symbol.HasAttribute())
		{
			state->SetNextAddress(boost::none);
		}
		else
		{
			state->SetNextAddress(m_table.GetStatesCount() + 1);
		}

		state->SetAcceptableTerminals({ symbol.GetName() });
		state->SetName(symbol.GetName());
		return state;
	}

	std::unique_ptr<ParserState> OnNonterminal(const IGrammarSymbol& symbol, int row, int col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Nonterminal);
		auto state = std::make_unique<ParserState>();

		int flags = StateFlags::Error;
		auto production = m_grammar.GetProduction(row);
		if (!production.IsLastSymbol(col) || symbol.HasAttribute())
		{
			flags |= StateFlags::Push;
		}
		state->SetFlags(flags);

		state->SetNextAddress(GetProductionIndex(m_grammar, symbol.GetName()));
		state->SetAcceptableTerminals(GatherAcceptablesAndFollowingsIfHasEmptiness(m_grammar, symbol.GetName()));
		state->SetName(symbol.GetName());
		return state;
	}

	std::unique_ptr<ParserState> OnEpsilon(const IGrammarSymbol& symbol, int row, int col)
	{
		assert(symbol.GetType() == GrammarSymbolType::Epsilon);
		auto state = std::make_unique<ParserState>();
		int flags = StateFlags::Error;
		state->SetFlags(flags);
		using boost::make_optional;
		state->SetNextAddress(symbol.HasAttribute() ? make_optional(m_table.GetStatesCount() + 1) : boost::none);
		state->SetAcceptableTerminals(GatherBeginningSymbolsOfProduction(m_grammar, row));
		state->SetName(symbol.GetName());
		return state;
	}

private:
	IGrammar& m_grammar;
	IParserTable& m_table;
};
}

using namespace grammarlib;

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

std::unique_ptr<IParserTable> ParserTable::Create(const grammarlib::IGrammar& grammar)
{
	auto table = std::make_unique<ParserTable>();
	auto factory = std::make_unique<ParserStateFactory>(grammar, *table);

	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		auto state = std::make_unique<ParserState>();
		state->SetFlags(!ProductionHasAlternativeWithHigherIndex(grammar, i) ? StateFlags::Error : 0);
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
				table->AddState(factory->CreateState(symbol, row, col));
				break;
			case GrammarSymbolType::Nonterminal:
				state->SetName(symbol.GetName());
				state->SetShiftFlag(false);
				state->SetPushFlag(col != (production.GetSymbolsCount() - 1) || bool(symbol.GetAttribute()));
				state->SetErrorFlag(true);
				state->SetEndFlag(false);
				state->SetNextAddress(GetProductionIndex(grammar, symbol.GetName()));
				state->SetAcceptableTerminals(GatherAcceptablesAndFollowingsIfHasEmptiness(grammar, symbol.GetName()));
				break;
			case GrammarSymbolType::Epsilon:
				state->SetName(symbol.GetName());
				state->SetShiftFlag(false);
				state->SetPushFlag(false);
				state->SetErrorFlag(true);
				state->SetEndFlag(false);
				if (bool(symbol.GetAttribute()))
				{
					state->SetNextAddress(table->GetStatesCount() + 1);
				}
				else
				{
					state->SetNextAddress(boost::none);
				}
				state->SetAcceptableTerminals(GatherBeginningSymbolsOfProduction(grammar, int(row)));
				break;
			}

			

			if (auto attribute = symbol.GetAttribute())
			{
				auto attributeState = std::make_unique<ParserState>();
				attributeState->SetName(*attribute);
				attributeState->SetShiftFlag(symbol.GetType() == GrammarSymbolType::Terminal);
				attributeState->SetPushFlag(false);
				attributeState->SetErrorFlag(false);
				attributeState->SetEndFlag(symbol.GetName() == grammar.GetEndTerminal());
				if (col == production.GetSymbolsCount() - 1)
				{
					attributeState->SetNextAddress(boost::none);
				}
				else
				{
					attributeState->SetNextAddress(table->GetStatesCount() + 1);
				}
				table->AddState(std::move(attributeState));
				++attributesCount;
			}
		}

		// Adding index that we skipped on first loop
		table->GetState(row).SetNextAddress(table->GetStatesCount() - production.GetSymbolsCount() - attributesCount);
	}

	return table;
}
