#include "stdafx.h"
#include "Grammar.h"
#include "GrammarUtils.h"

#include <cassert>
#include <iterator>
#include <boost/format.hpp>

using namespace grammarlib;

void Grammar::Normalize()
{
	if (IsEmpty())
	{
		throw std::logic_error("can't normalize empty grammar");
	}

	std::set<size_t> allIndices;
	for (size_t i = 0; i < GetProductionsCount(); ++i)
	{
		allIndices.insert(i);
	}

	std::vector<size_t> newIndices;
	while (!allIndices.empty())
	{
		size_t index = *allIndices.begin();
		allIndices.erase(allIndices.begin());

		const IGrammarProduction& production = *m_productions[index];
		std::set<int> alternatives = grammarlib::GatherProductionIndices(*this, production.GetLeftPart());

		for (int alternative : alternatives)
		{
			newIndices.push_back(size_t(alternative));
			allIndices.erase(size_t(alternative));
		}
	}

	std::vector<std::unique_ptr<IGrammarProduction>> productions;
	for (size_t i = 0; i < m_productions.size(); ++i)
	{
		productions.push_back(nullptr);
	}

	for (size_t i = 0; i < m_productions.size(); ++i)
	{
		productions[i] = std::move(m_productions[newIndices[i]]);
	}
	m_productions = std::move(productions);
}

void Grammar::AddProduction(std::unique_ptr<IGrammarProduction> && production)
{
	// First production must have ending terminal
	if (m_productions.empty() && !production->EndsWith(GrammarSymbolType::Terminal))
	{
		auto fmt = boost::format("grammar's first production should end with terminal, '%1%' given")
			% production->GetBackSymbol().GetName();
		throw std::invalid_argument(fmt.str());
	}

	// Other starting productions must have corrent ending terminal too
	if (!m_productions.empty() && production->GetLeftPart() == GetStartSymbol())
	{
		if (!production->EndsWith(GrammarSymbolType::Terminal))
		{
			auto fmt = boost::format("grammar's start symbol production must end with terminal, '%1%' given")
				% production->GetBackSymbol().GetName();
			throw std::runtime_error(fmt.str());
		}
		else if (production->GetBackSymbol().GetName() != GetEndTerminal())
		{
			auto fmt = boost::format("grammar can't have another ending terminal: '%1%'")
				% production->GetBackSymbol().GetName();
			throw std::runtime_error(fmt.str());
		}
	}

	// No grammar invalidation, can successfully add production
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

bool grammarlib::Grammar::IsEmpty() const
{
	return m_productions.empty();
}

const std::string& Grammar::GetStartSymbol() const
{
	if (m_productions.empty())
	{
		throw std::runtime_error("empty grammar can't have starting nonterminal");
	}
	return m_productions.front()->GetLeftPart();
}

const std::string& Grammar::GetEndTerminal() const
{
	if (m_productions.empty())
	{
		throw std::logic_error("empty grammar can't have ending terminal");
	}

	const IGrammarProduction& production = *m_productions.front();
	const IGrammarSymbol& symbol = production.GetBackSymbol();
	assert(symbol.GetType() == GrammarSymbolType::Terminal);

	return symbol.GetName();
}
