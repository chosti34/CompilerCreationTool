#include "pch.h"
#include "LanguageInformation.h"
#include "../Grammar/GrammarUtils.h"

using namespace grammarlib;

namespace
{
bool HasAcceptableSetsCrossing(
	const std::string& nonterminal,
	const IParserTable& table,
	const IGrammar& grammar
)
{
	std::set<std::string> nonterminalAcceptables;

	for (int index : GatherProductionIndices(grammar, nonterminal))
	{
		assert(table.GetState(index).GetAcceptableTerminals());
		const auto& stateAcceptables = *table.GetState(index).GetAcceptableTerminals();

		for (const std::string& acceptable : stateAcceptables)
		{
			auto inserted = nonterminalAcceptables.emplace(acceptable);
			if (!inserted.second)
			{
				return true;
			}
		}
	}

	return false;
}
}

LanguageInformation::LanguageInformation(
	const ILexer& lexer,
	const IParser<bool>& parser,
	const grammarlib::IGrammar& grammar
)
{
	m_actionsCount = parser.GetActionsCount();
	m_terminalsCount = lexer.GetPatternsCount();
	m_productionsCount = grammar.GetProductionsCount();

	m_emptyProductionsCount = CountProductions(grammar, [](const IGrammarProduction& production) {
		return production.GetSymbolsCount() == 1 && production.EndsWith(GrammarSymbolType::Epsilon);
	});

	const std::vector<std::string> nonterminals = GatherSymbols(grammar, [](const IGrammarSymbol& symbol) {
		return symbol.GetType() == GrammarSymbolType::Nonterminal;
	});

	// ��������� ����������� � �������������� ������������� �����������
	for (const std::string& nonterminal : nonterminals)
	{
		if (HasAcceptableSetsCrossing(nonterminal, parser.GetTable(), grammar))
		{
			m_acceptablesCrossingNonterminals.emplace(nonterminal);
		}
	}

	// ��������� ��������� ��������������� ������������
	for (const std::string& nonterminal : nonterminals)
	{
		if (HasLeftRecursion(grammar, nonterminal))
		{
			m_leftRecursiveNonterminals.emplace(nonterminal);
		}
	}
}

unsigned LanguageInformation::GetActionsCount() const
{
	return m_actionsCount;
}

unsigned LanguageInformation::GetTerminalsCount() const
{
	return m_terminalsCount;
}

unsigned LanguageInformation::GetProductionsCount() const
{
	return m_productionsCount;
}

unsigned LanguageInformation::GetEmptyProductionsCount() const
{
	return m_emptyProductionsCount;
}

std::unordered_set<std::string> const& LanguageInformation::GetLeftRecursiveNonterminals()
{
	return m_leftRecursiveNonterminals;
}

std::unordered_set<std::string> const& LanguageInformation::GetAcceptablesCrossingNonterminals()
{
	return m_acceptablesCrossingNonterminals;
}
