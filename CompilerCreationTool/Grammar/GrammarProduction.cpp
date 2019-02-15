#include "stdafx.h"
#include "GrammarProduction.h"

using namespace grammarlib;

GrammarProduction::GrammarProduction(
	const std::string& left,
	std::vector<std::unique_ptr<IGrammarSymbol>> && right
)
	: m_left(left)
	, m_right(std::move(right))
{
	if (m_right.empty())
	{
		throw std::runtime_error("grammar production's right part can't be empty");
	}
}

const std::string& GrammarProduction::GetLeftPart() const
{
	return m_left;
}

size_t GrammarProduction::GetSymbolsCount() const
{
	return m_right.size();
}

const IGrammarSymbol& GrammarProduction::GetSymbol(size_t index) const
{
	if (index >= m_right.size())
	{
		throw std::out_of_range("index must be less than entities count");
	}
	return *m_right[index];
}

bool GrammarProduction::EndsWith(GrammarSymbolType type) const
{
	return GetBackSymbol().GetType() == type;
}

bool grammarlib::GrammarProduction::IsLastSymbol(size_t index) const
{
	assert(!m_right.empty());
	return index == (m_right.size() - 1);
}

const IGrammarSymbol& GrammarProduction::GetFrontSymbol() const
{
	assert(!m_right.empty());
	return *m_right.front();
}

const IGrammarSymbol& grammarlib::GrammarProduction::GetBackSymbol() const
{
	assert(!m_right.empty());
	return *m_right.back();
}
