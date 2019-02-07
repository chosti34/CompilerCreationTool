#pragma once
#include "Fwd.h"
#include <memory>
#include <string>

namespace grammarlib
{
class ProductionFactory
{
public:
	std::unique_ptr<Production> CreateProduction(const std::string& line);
};
}
