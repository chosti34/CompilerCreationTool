#include "stdafx.h"
#include "GrammarBuilder.h"
#include "Grammar.h"
#include "GrammarProductionFactory.h"
#include <sstream>
#include <cctype>

using namespace grammarlib;

namespace
{
bool ConsistsOfWhitespaces(const std::string& line)
{
	return std::all_of(line.begin(), line.end(), std::isspace);
}
}

namespace grammarlib
{
std::unique_ptr<IGrammar> GrammarBuilder::CreateGrammar(const std::string& text)
{
	auto grammar = std::make_unique<Grammar>(text);
	auto factory = std::make_unique<GrammarProductionFactory>();

	std::istringstream strm(text);
	std::string line;

	while (getline(strm, line))
	{
		if (!line.empty() && !ConsistsOfWhitespaces(line))
		{
			grammar->AddProduction(factory->CreateProduction(line));
		}
	}

	if (grammar->IsEmpty())
	{
		throw std::invalid_argument("no grammar's productions provided");
	}

	grammar->NormalizeIndices();
	return grammar;
}
}
