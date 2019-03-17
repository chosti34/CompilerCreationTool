#pragma once
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Parser/ParseResults.h"
#include "../Grammar/IGrammar.h"
#include <unordered_set>
#include <chrono>

class LanguageInformation
{
public:
	LanguageInformation(
		const ILexer& lexer,
		const IParser<ParseResults>& parser,
		const grammarlib::IGrammar& grammar,
		const std::chrono::duration<double>& buildTime
	);

	std::chrono::duration<double> const& GetBuildTime() const;

	unsigned GetActionsCount() const;
	unsigned GetTerminalsCount() const;
	unsigned GetProductionsCount() const;
	unsigned GetEmptyProductionsCount() const;

	std::unordered_set<std::string> const& GetLeftRecursiveNonterminals() const;
	std::unordered_set<std::string> const& GetAcceptablesCrossingNonterminals() const;

private:
	std::chrono::duration<double> m_buildTime;
	unsigned m_actionsCount;
	unsigned m_terminalsCount;
	unsigned m_productionsCount;
	unsigned m_emptyProductionsCount;
	std::unordered_set<std::string> m_leftRecursiveNonterminals;
	std::unordered_set<std::string> m_acceptablesCrossingNonterminals;
};
