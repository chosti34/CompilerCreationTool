#include "stdafx.h"
#include "Parser.h"
#include "Action.h"
#include "ParserState.h"
#include "../Utils/string_utils.h"

#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace
{
void Log(IParserLogger* logger, const std::string& message)
{
	if (logger)
	{
		logger->Log(message);
	}
}

class ParserActionPerformer
{
public:
	ParserActionPerformer(IParserLogger* logger)
		: mLogger(logger)
	{
	}

	void DoNoAction()
	{
	}

	void DoPrintHello()
	{
		Log(mLogger, "Hello\n");
	}

	void DoPrintWorld()
	{
		Log(mLogger, "World\n");
	}

private:
	IParserLogger* mLogger;
};
}

Parser::Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer)
	: m_table(std::move(table))
	, m_lexer(lexer)
{
}

bool Parser::Parse(const std::string& text)
{
	ParserActionPerformer performer(mLogger.get());
	const std::map<ActionType, std::function<void()>> cFunctionMap = {
		{ ActionType::None, std::bind(&ParserActionPerformer::DoNoAction, &performer ) },
		{ ActionType::PrintHello, std::bind(&ParserActionPerformer::DoPrintHello, &performer) },
		{ ActionType::PrintWorld, std::bind(&ParserActionPerformer::DoPrintWorld, &performer) }
	};

	size_t index = 0;
	std::vector<size_t> addresses;

	m_lexer.SetText(text);
	Token token = m_lexer.GetNextToken();
	Log(mLogger.get(), "[#" + std::to_string(index) + "] " + "Token '" + token.name + "' read by lexer\n");

	while (true)
	{
		const IParserState& state = m_table->GetState(index);

		if (state.GetFlag(StateFlag::Attribute))
		{
			auto found = FindActionIndexByName(state.GetName());
			assert(found.is_initialized());
			auto& func = cFunctionMap.at(m_actions[*found]->GetType());
			Log(mLogger.get(), "[#" + std::to_string(index) + "]" + "Performing action '" + state.GetName() + "'...\n");
			func();
		}
		else if (!state.AcceptsTerminal(token.name))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				Log(mLogger.get(), "[#" + std::to_string(index) + "] " + "Parser doesn't accept token '" + token.name + "'\n");
				Log(mLogger.get(), "List of acceptable tokens: " +
					string_utils::JoinStrings(*state.GetAcceptableTerminals(), ", ", "[", "]") + "\n");
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
			Log(mLogger.get(), "[#" + std::to_string(index) + "] " + "Token '" + token.name + "' read by lexer\n");
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

IAction& Parser::GetAction(size_t index)
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

void Parser::SetLogger(std::unique_ptr<IParserLogger> && logger)
{
	mLogger = std::move(logger);
}

const IParserLogger* Parser::GetLogger() const
{
	return mLogger.get();
}

IParserLogger* Parser::GetLogger()
{
	return mLogger.get();
}

boost::optional<size_t> Parser::FindActionIndexByName(const std::string& name)
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
