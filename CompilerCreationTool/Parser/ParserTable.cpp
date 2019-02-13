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

std::unique_ptr<IParserTable> ParserTable::Create(const grammarlib::IGrammar& grammar)
{
	auto table = std::make_unique<ParserTable>();

	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		auto state = std::make_unique<ParserState>();
		state->SetPushFlag(false);
		state->SetShiftFlag(false);
		state->SetErrorFlag(!grammarlib::ProductionHasAlternativeWithHigherIndex(grammar, i));
		state->SetEndFlag(false);
		state->SetName(grammar.GetProduction(i).GetLeftPart());
		state->SetNextAddress(boost::none); // Address will be reassigned later
		state->SetAcceptableTerminals(grammarlib::GatherBeginningSymbolsOfProduction(grammar, int(i)));
		table->AddState(std::move(state));
	}

	for (size_t row = 0; row < grammar.GetProductionsCount(); ++row)
	{
		const grammarlib::IGrammarProduction& production = grammar.GetProduction(row);
		unsigned attributesCount = 0;

		for (size_t col = 0; col < production.GetSymbolsCount(); ++col)
		{
			const grammarlib::IGrammarSymbol& symbol = production.GetSymbol(col);
			auto state = std::make_unique<ParserState>();

			switch (symbol.GetType())
			{
			case GrammarSymbolType::Terminal:
				state->SetName(symbol.GetName());
				// Always shift when terminal doesn't have attribute
				state->SetShiftFlag(!bool(symbol.GetAttribute()));
				state->SetPushFlag(false);
				state->SetErrorFlag(true);
				// If symbol with end terminal have attribute, it will not have ending state
				state->SetEndFlag(symbol.GetName() == grammar.GetEndTerminal() && !bool(symbol.GetAttribute()));
				// If symbol is last in production AND it doesn't have attribute, there is no next address for this state
				if (col == production.GetSymbolsCount() - 1 && !bool(symbol.GetAttribute()))
				{
					state->SetNextAddress(boost::none);
				}
				else
				{
					state->SetNextAddress(table->GetStatesCount() + 1);
				}
				state->SetAcceptableTerminals({ symbol.GetName() });
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

			table->AddState(std::move(state));
			if (auto attribute = symbol.GetAttribute())
			{
				auto attributeState = std::make_unique<ParserState>();
				attributeState->SetName(*attribute);
				// If attribute for symbol of terminal type, then we need to shift on this state
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
