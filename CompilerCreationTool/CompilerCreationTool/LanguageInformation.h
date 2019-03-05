#pragma once
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Grammar/IGrammar.h"
#include <unordered_set>

class LanguageInformation
{
public:
	LanguageInformation(
		const ILexer& lexer,
		const IParser<bool>& parser,
		const grammarlib::IGrammar& grammar
	);

	unsigned GetActionsCount() const;
	unsigned GetTerminalsCount() const;
	unsigned GetProductionsCount() const;
	unsigned GetEmptyProductionsCount() const;

	std::unordered_set<std::string> const& GetLeftRecursiveNonterminals();
	std::unordered_set<std::string> const& GetAcceptablesCrossingNonterminals();

private:
	unsigned m_actionsCount;
	unsigned m_terminalsCount;
	unsigned m_productionsCount;
	unsigned m_emptyProductionsCount;
	std::unordered_set<std::string> m_leftRecursiveNonterminals;
	std::unordered_set<std::string> m_acceptablesCrossingNonterminals;
};
