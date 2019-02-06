#pragma once
#include "Production.h"
#include <memory>

namespace grammarlib
{
class Grammar
{
public:
	void AddProduction(std::unique_ptr<Production> && production);

	const Production& GetProduction(size_t index) const;
	size_t GetProductionsCount() const;

	const std::string& GetStartingNonterminal() const;
	const std::string& GetEndingTerminal() const;

private:
	std::vector<std::unique_ptr<Production>> m_productions;
};
}
