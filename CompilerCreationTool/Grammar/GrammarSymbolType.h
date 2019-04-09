#pragma once
#include <string>

enum class GrammarSymbolType
{
	Epsilon,
	Terminal,
	Nonterminal
};

std::string ToString(GrammarSymbolType type);
GrammarSymbolType ToGrammarSymbolType(const std::string& str);
