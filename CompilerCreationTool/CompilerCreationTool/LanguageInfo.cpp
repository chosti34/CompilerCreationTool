#include "pch.h"
#include "LanguageInfo.h"
#include "../Grammar/GrammarUtils.h"

using namespace grammarlib;

namespace
{
bool HasAcceptableSetsCrossing(const std::string& nonterminal, const IParserTable& table, const IGrammar& grammar)
{
	std::set<std::string> nonterminalAcceptables;
	for (int index : GatherProductionIndices(grammar, nonterminal))
	{
		const auto& stateAcceptables = table.GetState(index).GetAcceptableTerminals();
		for (const std::string& acceptable : stateAcceptables)
		{
			const auto inserted = nonterminalAcceptables.emplace(acceptable);
			if (!inserted.second)
			{
				return true;
			}
		}
	}
	return false;
}
}

LanguageInfo::LanguageInfo(
	const ILexer& lexer,
	const IParser<ParseResults>& parser,
	const grammarlib::IGrammar& grammar,
	const std::chrono::duration<double>& buildTime
)
{
	m_buildTime = buildTime;

	m_actionsCount = parser.GetActionsCount();
	m_terminalsCount = lexer.GetPatternsCount();
	m_productionsCount = grammar.GetProductionsCount();

	m_emptyProductionsCount = CountProductions(grammar, [](const IGrammarProduction& production) {
		return production.GetSymbolsCount() == 1 && production.EndsWith(GrammarSymbolType::Epsilon);
	});

	const std::vector<std::string> nonterminals = GatherAllNonterminals(grammar);

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

std::chrono::duration<double> const& LanguageInfo::GetBuildTime() const
{
	return m_buildTime;
}

unsigned LanguageInfo::GetActionsCount() const
{
	return m_actionsCount;
}

unsigned LanguageInfo::GetTerminalsCount() const
{
	return m_terminalsCount;
}

unsigned LanguageInfo::GetProductionsCount() const
{
	return m_productionsCount;
}

unsigned LanguageInfo::GetEmptyProductionsCount() const
{
	return m_emptyProductionsCount;
}

std::unordered_set<std::string> const& LanguageInfo::GetLeftRecursiveNonterminals() const
{
	return m_leftRecursiveNonterminals;
}

std::unordered_set<std::string> const& LanguageInfo::GetAcceptablesCrossingNonterminals() const
{
	return m_acceptablesCrossingNonterminals;
}
