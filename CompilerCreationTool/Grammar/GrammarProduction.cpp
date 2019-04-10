#include "stdafx.h"
#include "GrammarProduction.h"

using namespace grammarlib;

namespace
{
bool HasEpsilon(const std::vector<std::unique_ptr<IGrammarSymbol>> &right)
{
	for (const auto& symbol : right)
	{
		if (symbol->GetType() == GrammarSymbolType::Epsilon)
		{
			return true;
		}
	}
	return false;
}

void ValidateRightPart(const std::vector<std::unique_ptr<IGrammarSymbol>> &right)
{
	// Правая часть правила не может быть пустой
	if (right.empty())
	{
		throw std::runtime_error("production's right part can't be empty");
	}
	// Правило может содержать только один пустой символ
	if (HasEpsilon(right) && right.size() != 1)
	{
		throw std::runtime_error("epsilon production can't contain any additional symbols");
	}
}
}

GrammarProduction::GrammarProduction(
	const std::string& left,
	std::vector<std::unique_ptr<IGrammarSymbol>> && right
)
	: m_left(left)
	, m_right(std::move(right))
{
	ValidateRightPart(m_right);
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
