#include "stdafx.h"
#include "Grammar.h"
#include "GrammarUtils.h"
#include <cassert>
#include <iterator>

using namespace grammarlib;

namespace
{
std::set<size_t> CreateIotaSet(size_t count, size_t value = 0)
{
	std::set<size_t> set;
	while (value < count)
	{
		set.insert(value++);
	}
	return set;
}

bool operator ==(const IGrammarSymbol& lhs, const IGrammarSymbol& rhs)
{
	return lhs.GetName() == rhs.GetName() && lhs.GetType() == rhs.GetType();
}

bool LastSymbolRepeats(const IGrammarProduction& production)
{
	for (size_t i = 0; i < production.GetSymbolsCount() - 1; ++i)
	{
		if (production.GetSymbol(i) == production.GetBackSymbol())
		{
			return true;
		}
	}
	return false;
}

bool HasSymbol(const IGrammarProduction& production, const IGrammarSymbol& symbol)
{
	for (size_t i = 0; i < production.GetSymbolsCount(); ++i)
	{
		if (production.GetSymbol(i) == symbol)
		{
			return true;
		}
	}
	return false;
}
}

Grammar::Grammar(const std::string& text)
	: mText(text)
{
}

bool Grammar::IsNormalized() const
{
	return mNormalized;
}

// Делаем все альтернативны нетерминала соседними по индексу в массиве правил грамматики
void Grammar::NormalizeIndices()
{
	if (IsEmpty())
	{
		throw std::logic_error("can't normalize indices of empty grammar");
	}

	std::set<size_t> indices = CreateIotaSet(GetProductionsCount());
	std::vector<size_t> newIndices;
	newIndices.reserve(indices.size());

	// Собираем массив с индексами с "исправленным" порядком
	while (!indices.empty())
	{
		const size_t index = *indices.begin();
		indices.erase(indices.begin());

		const IGrammarProduction& production = *mProductions[index];
		const std::set<int> alternatives = GatherProductionIndices(*this, production.GetLeftPart());

		for (const int alternative : alternatives)
		{
			newIndices.push_back(size_t(alternative));
			indices.erase(size_t(alternative));
		}
	}

	// Переставляем порядок правил в соответствии с новым массивом индексов
	std::vector<bool> swapped(mProductions.size(), false);
	for (size_t i = 0; i < mProductions.size(); ++i)
	{
		if (i != newIndices[i] && !swapped[i] && !swapped[newIndices[i]])
		{
			std::swap(mProductions[i], mProductions[newIndices[i]]);
			swapped[i] = swapped[newIndices[i]] = true;
		}
	}

	mNormalized = true;
}

void Grammar::AddProduction(std::unique_ptr<IGrammarProduction> && production)
{
	if (IsEmpty())
	{
		TryInsertFirstProduction(std::move(production));
	}
	else
	{
		TryInsertProduction(std::move(production));
	}
}

const IGrammarProduction& Grammar::GetProduction(size_t index) const
{
	if (index >= mProductions.size())
	{
		throw std::out_of_range("index must be less than productions count");
	}
	return *mProductions[index];
}

size_t Grammar::GetProductionsCount() const
{
	return mProductions.size();
}

bool Grammar::IsEmpty() const
{
	return mProductions.empty();
}

const std::string& Grammar::GetText() const
{
	return mText;
}

const std::string& Grammar::GetStartSymbol() const
{
	if (mProductions.empty())
	{
		throw std::logic_error("empty grammar can't have starting nonterminal");
	}
	return mProductions.front()->GetLeftPart();
}

const std::string& Grammar::GetEndTerminal() const
{
	if (mProductions.empty())
	{
		throw std::logic_error("empty grammar can't have ending terminal");
	}

	const IGrammarProduction& production = *mProductions.front();
	const IGrammarSymbol& symbol = production.GetBackSymbol();

	assert(symbol.GetType() == GrammarSymbolType::Terminal);
	return symbol.GetName();
}

void Grammar::TryInsertFirstProduction(std::unique_ptr<IGrammarProduction> && production)
{
	assert(IsEmpty());

	if (!production->EndsWith(GrammarSymbolType::Terminal))
	{
		throw std::runtime_error("grammar's first production must end with terminal, " +
			production->GetBackSymbol().GetName() + " given");
	}

	if (LastSymbolRepeats(*production))
	{
		throw std::runtime_error("grammar's ending symbol '" +
			production->GetBackSymbol().GetName() + "' can't be repeated");
	}

	mProductions.push_back(std::move(production));
	mNormalized = false;
}

void Grammar::TryInsertProduction(std::unique_ptr<IGrammarProduction> && production)
{
	assert(!IsEmpty());

	if (production->GetLeftPart() == GetStartSymbol())
	{
		if (!production->EndsWith(GrammarSymbolType::Terminal))
		{
			throw std::runtime_error("grammar's first production must end with terminal, " +
				production->GetBackSymbol().GetName() + " given");
		}
		if (production->GetBackSymbol().GetName() != GetEndTerminal())
		{
			throw std::runtime_error("grammar's ending terminal must match '" + GetEndTerminal() + "', but '" +
				production->GetBackSymbol().GetName() + "' given");
		}
		if (LastSymbolRepeats(*production))
		{
			throw std::runtime_error("grammar's ending symbol '" +
				production->GetBackSymbol().GetName() + "' can't be repeated");
		}
	}
	else if (HasSymbol(*production, mProductions.front()->GetBackSymbol()))
	{
		throw std::runtime_error(
			"grammar's ending symbol can't be located at production #" + std::to_string(GetProductionsCount())
		);
	}

	mProductions.push_back(std::move(production));
	mNormalized = false;
}
