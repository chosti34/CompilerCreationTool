#pragma once
#include "IGrammarProduction.h"
#include <memory>

namespace grammarlib
{
class IGrammarProductionFactory
{
public:
	virtual ~IGrammarProductionFactory() = default;
	virtual std::unique_ptr<IGrammarProduction> CreateProduction(const std::string& line) = 0;
};
}
