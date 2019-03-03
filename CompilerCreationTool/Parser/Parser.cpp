#include "stdafx.h"
#include "Parser.h"
#include "ParserState.h"
#include "Action.h"
#include "../Utils/string_utils.h"

#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace
{
void NoAction()
{
	std::cout << "No action" << std::endl;
}

void PrintHello()
{
	std::cout << "Hello, ";
}

void PrintWorld()
{
	std::cout << "World" << std::endl;
}

const std::map<ActionType, std::function<void()>> gcFunctionsMap = {
	{ ActionType::None, NoAction },
	{ ActionType::PrintHello, PrintHello },
	{ ActionType::PrintWorld, PrintWorld }
};
}

Parser::Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer)
	: m_table(std::move(table))
	, m_lexer(lexer)
{
}

bool Parser::Parse(const std::string& text)
{
	m_lexer.SetText(text);
	Token token = m_lexer.GetNextToken();

	size_t index = 0;
	std::vector<size_t> addresses;

	while (true)
	{
		const IParserState& state = m_table->GetState(index);

		if (state.GetFlag(StateFlag::Attribute))
		{
			auto index = GetActionIndex(state.GetName());
			assert(index);
			auto& fn = gcFunctionsMap.at(m_actions[*index]->GetType());
			fn();
		}
		else if (!state.AcceptsTerminal(token.name))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				return false;
			}
			else
			{
				++index;
				continue;
			}
		}

		if (state.GetFlag(StateFlag::End))
		{
			assert(addresses.empty());
			return true;
		}
		if (state.GetFlag(StateFlag::Push))
		{
			addresses.push_back(index + 1);
		}
		if (state.GetFlag(StateFlag::Shift))
		{
			token = m_lexer.GetNextToken();
		}

		if (auto next = state.GetNextAddress())
		{
			index = *next;
		}
		else
		{
			assert(!addresses.empty());
			index = addresses.back();
			addresses.pop_back();
		}
	}

	assert(false);
	throw std::logic_error("unreachable code in parse function");
}

const IParserTable& Parser::GetTable() const
{
	return *m_table;
}

void Parser::SetActionNames(const std::vector<std::string> &actions)
{
	for (const std::string& name : actions)
	{
		m_actions.emplace_back(std::make_unique<Action>(name, ActionType::None));
	}
}

void Parser::SetAction(size_t index, std::unique_ptr<IAction> && action)
{
	if (index >= m_actions.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	m_actions[index] = std::move(action);
}

void Parser::SwapActions(size_t oldPos, size_t newPos)
{
	std::iter_swap(m_actions.begin() + oldPos, m_actions.begin() + newPos);
}

const IAction& Parser::GetAction(size_t index) const
{
	if (index >= m_actions.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *m_actions[index];
}

IAction & Parser::GetAction(size_t index)
{
	if (index >= m_actions.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *m_actions[index];
}

size_t Parser::GetActionsCount() const
{
	return m_actions.size();
}

boost::optional<size_t> Parser::GetActionIndex(const std::string& name)
{
	for (size_t i = 0; i < m_actions.size(); ++i)
	{
		const auto& pAction = m_actions[i];
		if (pAction->GetName() == name)
		{
			return boost::make_optional(i);
		}
	}
	return boost::none;
}
