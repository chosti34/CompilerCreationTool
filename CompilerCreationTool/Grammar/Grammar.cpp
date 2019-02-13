#include "stdafx.h"
#include "Grammar.h"
#include <cassert>

using namespace grammarlib;

void Grammar::AddProduction(std::unique_ptr<IGrammarProduction> && production)
{
	if (m_productions.empty() && !production->EndsWith(GrammarSymbolType::Terminal))
	{
		throw std::runtime_error("grammar's first production should end with terminal");
	}
	m_productions.push_back(std::move(production));
}

const IGrammarProduction& Grammar::GetProduction(size_t index) const
{
	if (index >= m_productions.size())
	{
		throw std::out_of_range("index must be less than productions count");
	}
	return *m_productions[index];
}

size_t Grammar::GetProductionsCount() const
{
	return m_productions.size();
}

const std::string& Grammar::GetStartSymbol() const
{
	if (m_productions.empty())
	{
		throw std::runtime_error("empty grammar does't have starting nonterminal");
	}
	return m_productions.front()->GetLeftPart();
}

const std::string& Grammar::GetEndTerminal() const
{
	if (m_productions.empty())
	{
		throw std::runtime_error("empty grammar doesn't have ending terminal");
	}

	const IGrammarProduction& production = *m_productions.front();
	const IGrammarSymbol& symbol = production.GetBackSymbol();

	assert(symbol.GetType() == GrammarSymbolType::Terminal);
	return symbol.GetName();
}
