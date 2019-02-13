#pragma once
#include "IGrammarProduction.h"
#include <memory>

namespace grammarlib
{
class IGrammar
{
public:
	virtual ~IGrammar() = default;

	virtual void AddProduction(std::unique_ptr<IGrammarProduction> && production) = 0;
	virtual const IGrammarProduction& GetProduction(size_t index) const = 0;
	virtual size_t GetProductionsCount() const = 0;

	virtual const std::string& GetStartSymbol() const = 0;
	virtual const std::string& GetEndTerminal() const = 0;
};
}
