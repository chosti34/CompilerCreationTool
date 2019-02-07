#include "stdafx.h"
#include "Utils.h"
#include "Grammar.h"
#include <cassert>
#include <stack>

namespace grammarlib
{
std::set<int> GatherProductionIndices(const Grammar& grammar, const std::string& nonterminal)
{
	std::set<int> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const auto production = grammar.GetProduction(i);
		if (production.GetLeftPart() == nonterminal)
		{
			indices.insert(static_cast<int>(i));
		}
	}
	return indices;
}

std::set<int> GatherProductionIndices(const Grammar& grammar, std::function<bool(const Production&)> && predicate)
{
	std::set<int> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const auto production = grammar.GetProduction(i);
		if (predicate(production))
		{
			indices.insert(static_cast<int>(i));
		}
	}
	return indices;
}

bool ProductionConsistsOfNonterminals(const Production& production)
{
	assert(production.GetEntitiesCount() != 0);
	for (size_t i = 0; i < production.GetEntitiesCount(); ++i)
	{
		const auto& entity = production.GetEntity(i);
		if (entity.GetType() != Entity::Nonterminal)
		{
			return false;
		}
	}
	return true;
}

bool ExistsEpsilonProduction(const Grammar& grammar, const std::string& nonterminal)
{
	for (const auto index : GatherProductionIndices(grammar, nonterminal))
	{
		const Production& production = grammar.GetProduction(index);
		if (production.GetFrontEntity().GetType() == Entity::Epsilon)
		{
			return true;
		}
	}
	return false;
}

bool NonterminalHasEmptiness(const Grammar& grammar, const std::string& nonterminal)
{
	if (ExistsEpsilonProduction(grammar, nonterminal))
	{
		return true;
	}

	const auto indices = GatherProductionIndices(grammar, [&nonterminal](const Production& production) -> bool {
		return production.GetLeftPart() == nonterminal && ProductionConsistsOfNonterminals(production);
	});

	// TODO: why compiler says unreachable code?
	for (const auto index : indices)
	{
		const Production& production = grammar.GetProduction(index);
		assert(production.GetEntitiesCount() != 0);

		for (size_t i = 0; i < production.GetEntitiesCount(); ++i)
		{
			const Entity& entity = production.GetEntity(i);
			assert(entity.GetType() == Entity::Nonterminal);

			if (!NonterminalHasEmptiness(grammar, entity.GetName()))
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

std::set<std::pair<int, int>> GatherNonterminalOccurrences(const Grammar& grammar, const std::string& nonterminal)
{
	std::set<std::pair<int, int>> indices;
	for (size_t i = 0; i < grammar.GetProductionsCount(); ++i)
	{
		const Production& production = grammar.GetProduction(i);
		for (size_t j = 0; j < production.GetEntitiesCount(); ++j)
		{
			const Entity& entity = production.GetEntity(j);
			if (entity.GetType() == Entity::Nonterminal && entity.GetName() == nonterminal)
			{
				indices.emplace(static_cast<int>(i), static_cast<int>(j));
			}
		}
	}
	return indices;
}

std::set<std::string> GatherBeginningSymbolsOfNonterminal(const Grammar& grammar, const std::string& nonterminal)
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
			const Production& production = grammar.GetProduction(productionIndex);
			const Entity& beginning = production.GetFrontEntity();

			if (beginning.GetType() == Entity::Terminal)
			{
				symbols.insert(beginning.GetName());
			}
			else if (beginning.GetType() == Entity::Nonterminal)
			{
				if (visited.find(beginning.GetName()) == visited.end())
				{
					visited.insert(beginning.GetName());
					stack.push(beginning.GetName());
				}

				bool hasEmptiness = NonterminalHasEmptiness(grammar, beginning.GetName());
				size_t index = 1u;

				while (index < production.GetEntitiesCount() && hasEmptiness)
				{
					const auto& current = production.GetEntity(index);
					if (current.GetType() == Entity::Terminal)
					{
						symbols.insert(current.GetName());
						break;
					}
					else if (current.GetType() == Entity::Nonterminal)
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

std::set<std::string> GatherBeginningSymbolsOfProduction(const Grammar& grammar, int productionIndex)
{
	assert(productionIndex < grammar.GetProductionsCount());
	const Entity& beginning = grammar.GetProduction(productionIndex).GetFrontEntity();

	if (beginning.GetType() == Entity::Terminal)
	{
		return { beginning.GetName() };
	}
	else if (beginning.GetType() == Entity::Nonterminal)
	{
		const auto production = grammar.GetProduction(productionIndex);

		std::set<std::string> symbols;
		std::set<std::string> expandables = { beginning.GetName() };

		bool hasEmtiness = NonterminalHasEmptiness(grammar, beginning.GetName());
		size_t index = 1u;

		while (index < production.GetEntitiesCount() && hasEmtiness)
		{
			const auto& current = production.GetEntity(index);
			if (current.GetType() == Entity::Terminal)
			{
				symbols.insert(current.GetName());
				break;
			}
			else if (current.GetType() == Entity::Nonterminal)
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
	else if (beginning.GetType() == Entity::Epsilon)
	{
		return GatherFollowingSymbols(grammar, grammar.GetProduction(productionIndex).GetLeftPart());
	}
	throw std::logic_error("unknown beginning symbol type: " + beginning.GetName());
}

std::set<std::string> GatherFollowingSymbols(const Grammar& grammar, const std::string& nonterminal)
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
			const Production& production = grammar.GetProduction(occurrence.first);
			if (occurrence.second == production.GetEntitiesCount() - 1u)
			{
				if (visited.find(production.GetLeftPart()) == visited.end())
				{
					stack.push(production.GetLeftPart());
					visited.insert(production.GetLeftPart());
				}
				continue;
			}

			size_t index = occurrence.second + 1u;
			const auto& symbol = production.GetEntity(index++);

			if (symbol.GetType() == Entity::Terminal)
			{
				followings.insert(symbol.GetName());
			}
			else if (symbol.GetType() == Entity::Nonterminal)
			{
				auto beginnings = GatherBeginningSymbolsOfNonterminal(grammar, symbol.GetName());
				followings.insert(beginnings.begin(), beginnings.end());

				// process case when that nonterminal can be empty
				bool hasEmptiness = NonterminalHasEmptiness(grammar, symbol.GetName());
				while (index < production.GetEntitiesCount() && hasEmptiness)
				{
					const Entity& current = production.GetEntity(index++);
					if (current.GetType() == Entity::Terminal)
					{
						followings.insert(current.GetName());
						break;
					}
					else if (current.GetType() == Entity::Nonterminal)
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

				const Entity& last = production.GetBackEntity();
				if (index >= production.GetEntitiesCount() &&
					last.GetType() == Entity::Nonterminal &&
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
