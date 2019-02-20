#pragma once
#include "IGrammar.h"

namespace grammarlib
{
class GrammarBuilder
{
public:
	std::unique_ptr<IGrammar> CreateGrammar(const std::string& text);
};
}
