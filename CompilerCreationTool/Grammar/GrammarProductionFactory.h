#pragma once
#include "IGrammarProductionFactory.h"

namespace grammarlib
{
class GrammarProductionFactory : public IGrammarProductionFactory
{
public:
	std::unique_ptr<IGrammarProduction> CreateProduction(const std::string& line) override;
};
}
