#include "stdafx.h"
#include "GrammarUtils.h"

#include <numeric>
#include <cassert>
#include <vector>
#include <stack>
#include <map>

namespace grammarlib
{
std::set<std::string> GatherAllTerminals(const IGrammar& grammar)
{
	std::set<std::string> terminals;
	for (size_t row = 0; row < grammar.GetProductionsCount(); ++row)
	{
		const IGrammarProduction& production = grammar.GetProduction(row);
		for (size_t col = 0; col < production.GetSymbolsCount(); ++col)
		{
			const IGrammarSymbol& symbol = production.GetSymbol(col);
			if (symbol.GetType() == GrammarSymbolType::Terminal)
			{
				terminals.insert(symbol.GetName());
			}
		}
	}
	return terminals;
}

std::set<std::string> GatherAllActions(const IGrammar& grammar)
{
	std::set<std::string> actions;
	for (size_t row = 0; row < grammar.GetProductionsCount(); ++row)
	{
		const IGrammarProduction& production = grammar.GetProduction(row);
		for (size_t col = 0; col < production.GetSymbolsCount(); ++col)
		{
			const IGrammarSymbol& symbol = production.GetSymbol(col);
			if (symbol.HasAttribute())
			{
				actions.insert(*symbol.GetAttribute());
			}
		}
	}
	return actions;
}

size_t GetProductionIndex(const IGrammar& grammar, const std::string& nonterminal)
{
	for (size_t index = 0; index < grammar.GetProductionsCount(); ++index)
	{
		if (grammar.GetProduction(index).GetLeftPart() == nonterminal)
		{
			return index;
		}
	}
	throw std::invalid_argument("grammar doesn't have nonterminal '" + nonterminal + "'");
}

bool ProductionHasAlternativeWithHigherIndex(const IGrammar & grammar, size_t index)
{
	const IGrammarProduction& lhs = grammar.GetProduction(index++);
	while (index < grammar.GetProductionsCount())
	{
		const IGrammarProduction& rhs = grammar.GetProduction(index++);
		if (lhs.GetLeftPart() == rhs.GetLeftPart())
		{
			return true;
		}
	}
	return false;
}

std::set<int> GatherProductionIndices(const IGrammar& grammar, const std::string& nonterminal)
{
	std::set<int> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const IGrammarProduction& production = grammar.GetProduction(i);
		if (production.GetLeftPart() == nonterminal)
		{
			indices.insert(static_cast<int>(i));
		}
	}
	return indices;
}

std::set<int> GatherProductionIndices(const IGrammar& grammar, std::function<bool(const IGrammarProduction&)> && predicate)
{
	std::set<int> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const IGrammarProduction& production = grammar.GetProduction(i);
		if (predicate(production))
		{
			indices.insert(static_cast<int>(i));
		}
	}
	return indices;
}

bool ProductionConsistsOfNonterminals(const IGrammarProduction& production)
{
	assert(production.GetSymbolsCount() != 0);
	for (size_t i = 0; i < production.GetSymbolsCount(); ++i)
	{
		const auto& entity = production.GetSymbol(i);
		if (entity.GetType() != GrammarSymbolType::Nonterminal)
		{
			return false;
		}
	}
	return true;
}

bool ExistsEpsilonProduction(const IGrammar& grammar, const std::string& nonterminal)
{
	for (const auto index : GatherProductionIndices(grammar, nonterminal))
	{
		const IGrammarProduction& production = grammar.GetProduction(index);
		if (production.GetFrontSymbol().GetType() == GrammarSymbolType::Epsilon)
		{
			return true;
		}
	}
	return false;
}

bool NonterminalHasEmptiness(const IGrammar& grammar, const std::string& nonterminal)
{
	if (ExistsEpsilonProduction(grammar, nonterminal))
	{
		return true;
	}

	const auto indices = GatherProductionIndices(grammar, [&nonterminal](const IGrammarProduction& production) -> bool {
		return production.GetLeftPart() == nonterminal && ProductionConsistsOfNonterminals(production);
	});

	// TODO: why compiler says unreachable code?
	for (const auto index : indices)
	{
		const IGrammarProduction& production = grammar.GetProduction(index);
		assert(production.GetSymbolsCount() != 0);

		for (size_t i = 0; i < production.GetSymbolsCount(); ++i)
		{
			const IGrammarSymbol& entity = production.GetSymbol(i);
			assert(entity.GetType() == GrammarSymbolType::Nonterminal);

			if (!NonterminalHasEmptiness(grammar, entity.GetName()))
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

std::set<std::pair<int, int>> GatherNonterminalOccurrences(const IGrammar& grammar, const std::string& nonterminal)
{
	std::set<std::pair<int, int>> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const IGrammarProduction& production = grammar.GetProduction(i);
		for (size_t j = 0; j < production.GetSymbolsCount(); ++j)
		{
			const IGrammarSymbol& entity = production.GetSymbol(j);
			if (entity.GetType() == GrammarSymbolType::Nonterminal && entity.GetName() == nonterminal)
			{
				indices.emplace(static_cast<int>(i), static_cast<int>(j));
			}
		}
	}
	return indices;
}

std::set<std::string> GatherBeginningSymbolsOfNonterminal(const IGrammar& grammar, const std::string& nonterminal)
{
	std::stack<std::string> stack;
	std::set<std::string> visited;
	std::set<std::string> symbols;

	stack.push(nonterminal);
	visited.insert(nonterminal);

	while (!stack.empty())
	{
		const std::string symbol = std::move(stack.top());
		stack.pop();

		for (const auto productionIndex : GatherProductionIndices(grammar, symbol))
		{
			const IGrammarProduction& production = grammar.GetProduction(productionIndex);
			const IGrammarSymbol& beginning = production.GetFrontSymbol();

			if (beginning.GetType() == GrammarSymbolType::Terminal)
			{
				symbols.insert(beginning.GetName());
			}
			else if (beginning.GetType() == GrammarSymbolType::Nonterminal)
			{
				if (visited.find(beginning.GetName()) == visited.end())
				{
					visited.insert(beginning.GetName());
					stack.push(beginning.GetName());
				}

				bool hasEmptiness = NonterminalHasEmptiness(grammar, beginning.GetName());
				size_t index = 1u;

				while (index < production.GetSymbolsCount() && hasEmptiness)
				{
					const auto& current = production.GetSymbol(index);
					if (current.GetType() == GrammarSymbolType::Terminal)
					{
						symbols.insert(current.GetName());
						break;
					}
					else if (current.GetType() == GrammarSymbolType::Nonterminal)
					{
						if (visited.find(current.GetName()) == visited.end())
						{
							visited.insert(current.GetName());
							stack.push(current.GetName());
						}
						++index;
						hasEmptiness = NonterminalHasEmptiness(grammar, current.GetName());
					}
					else
					{
						throw std::invalid_argument("this symbol can't appear after nonterminal: " + current.GetName());
					}
				}
			}
		}
	}

	return symbols;
}

std::set<std::string> GatherBeginningSymbolsOfProduction(const IGrammar& grammar, int productionIndex)
{
	assert(productionIndex < grammar.GetProductionsCount());
	const IGrammarSymbol& beginning = grammar.GetProduction(productionIndex).GetFrontSymbol();

	if (beginning.GetType() == GrammarSymbolType::Terminal)
	{
		return { beginning.GetName() };
	}
	else if (beginning.GetType() == GrammarSymbolType::Nonterminal)
	{
		const IGrammarProduction& production = grammar.GetProduction(productionIndex);

		std::set<std::string> symbols;
		std::set<std::string> expandables = { beginning.GetName() };

		bool hasEmtiness = NonterminalHasEmptiness(grammar, beginning.GetName());
		size_t index = 1u;

		while (index < production.GetSymbolsCount() && hasEmtiness)
		{
			const auto& current = production.GetSymbol(index);
			if (current.GetType() == GrammarSymbolType::Terminal)
			{
				symbols.insert(current.GetName());
				break;
			}
			else if (current.GetType() == GrammarSymbolType::Nonterminal)
			{
				expandables.insert(current.GetName());
				hasEmtiness = NonterminalHasEmptiness(grammar, current.GetName());
				++index;
			}
			else
			{
				throw std::invalid_argument("this symbol can't appear after nonterminal: " + current.GetName());
			}
		}

		while (!expandables.empty())
		{
			const auto node = *expandables.begin();
			expandables.erase(expandables.begin());

			const auto beginningSymbols = GatherBeginningSymbolsOfNonterminal(grammar, node);
			symbols.insert(beginningSymbols.begin(), beginningSymbols.end());
		}

		return symbols;
	}
	else if (beginning.GetType() == GrammarSymbolType::Epsilon)
	{
		return GatherFollowingSymbols(grammar, grammar.GetProduction(productionIndex).GetLeftPart());
	}
	throw std::logic_error("unknown beginning symbol type: " + beginning.GetName());
}

std::set<std::string> GatherFollowingSymbols(const IGrammar& grammar, const std::string& nonterminal)
{
	std::set<std::string> followings;
	std::stack<std::string> stack;
	std::set<std::string> visited;

	stack.push(nonterminal);
	visited.insert(nonterminal);

	while (!stack.empty())
	{
		const std::string node = std::move(stack.top());
		stack.pop();

		for (const std::pair<int, int> occurrence : GatherNonterminalOccurrences(grammar, node))
		{
			const IGrammarProduction& production = grammar.GetProduction(occurrence.first);
			if (occurrence.second == production.GetSymbolsCount() - 1u)
			{
				if (visited.find(production.GetLeftPart()) == visited.end())
				{
					stack.push(production.GetLeftPart());
					visited.insert(production.GetLeftPart());
				}
				continue;
			}

			size_t index = occurrence.second + 1u;
			const auto& symbol = production.GetSymbol(index++);

			if (symbol.GetType() == GrammarSymbolType::Terminal)
			{
				followings.insert(symbol.GetName());
			}
			else if (symbol.GetType() == GrammarSymbolType::Nonterminal)
			{
				auto beginnings = GatherBeginningSymbolsOfNonterminal(grammar, symbol.GetName());
				followings.insert(beginnings.begin(), beginnings.end());

				// process case when that nonterminal can be empty
				bool hasEmptiness = NonterminalHasEmptiness(grammar, symbol.GetName());
				while (index < production.GetSymbolsCount() && hasEmptiness)
				{
					const IGrammarSymbol& current = production.GetSymbol(index++);
					if (current.GetType() == GrammarSymbolType::Terminal)
					{
						followings.insert(current.GetName());
						break;
					}
					else if (current.GetType() == GrammarSymbolType::Nonterminal)
					{
						beginnings = GatherBeginningSymbolsOfNonterminal(grammar, nonterminal);
						followings.insert(beginnings.begin(), beginnings.end());
						hasEmptiness = NonterminalHasEmptiness(grammar, current.GetName());
					}
					else
					{
						throw std::invalid_argument("this symbol can't appear after terminal or nonterminal: " + symbol.GetName());
					}
				}

				const IGrammarSymbol& last = production.GetBackSymbol();
				if (index >= production.GetSymbolsCount() &&
					last.GetType() == GrammarSymbolType::Nonterminal &&
					NonterminalHasEmptiness(grammar, last.GetName()) &&
					visited.find(production.GetLeftPart()) == visited.end())
				{
					stack.push(production.GetLeftPart());
					visited.insert(production.GetLeftPart());
				}
			}
			else
			{
				throw std::invalid_argument("this symbol can't appear after terminal or nonterminal: " + symbol.GetName());
			}
		}
	}

	return followings;
}
}
