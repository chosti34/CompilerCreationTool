#include "stdafx.h"
#include "GrammarSymbolType.h"
#include <algorithm>
#include <cassert>
#include <vector>

namespace
{
const std::vector<std::pair<GrammarSymbolType, std::string>> gcTypeStringMapping = {
	{ GrammarSymbolType::Terminal, "Terminal" },
	{ GrammarSymbolType::Nonterminal, "Nonterminal" },
	{ GrammarSymbolType::Epsilon, "Epsilon" }
};
}

std::string ToString(GrammarSymbolType type)
{
	auto it = std::find_if(gcTypeStringMapping.begin(), gcTypeStringMapping.end(), [&](const auto& pair) {
		return pair.first == type;
	});

	if (it != gcTypeStringMapping.end())
	{
		return it->second;
	}

	assert(false);
	throw std::logic_error("can't convert unknown grammar symbol type to string");
}

GrammarSymbolType ToGrammarSymbolType(const std::string& str)
{
	auto it = std::find_if(gcTypeStringMapping.begin(), gcTypeStringMapping.end(), [&](const auto& pair) {
		return pair.second == str;
	});

	if (it != gcTypeStringMapping.end())
	{
		return it->first;
	}

	assert(false);
	throw std::logic_error("string '" + str + "' can't be converted to grammar symbol type");
}
