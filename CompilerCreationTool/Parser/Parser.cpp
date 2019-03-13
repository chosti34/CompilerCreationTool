#include "stdafx.h"
#include "Parser.h"
#include "Action.h"
#include "ParserState.h"
#include "../Utils/string_utils.h"

#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <boost/format.hpp>

using namespace std::literals::string_literals;

namespace
{
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
		if (mLogger)
		{
			mLogger->Log("Hello\n");
		}
	}

	void DoPrintWorld()
	{
		if (mLogger)
		{
			mLogger->Log("World\n");
		}
	}

private:
	IParserLogger* mLogger;
};
}

Parser::Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer)
	: mTable(std::move(table))
	, mLexer(lexer)
	, mActionList()
	, mLogger(nullptr)
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

	mLexer.SetText(text);
	Token token;

	try
	{
		token = FetchNextToken(index);
	}
	catch (const std::exception& ex)
	{
		LogIfNotNull("Lexer error: "s + ex.what(), index);
		return false;
	}

	while (true)
	{
		const IParserState& state = mTable->GetState(index);

		if (state.GetFlag(StateFlag::Attribute))
		{
			auto found = FindActionIndexByName(state.GetName());
			assert(found.is_initialized());
			auto& func = cFunctionMap.at(mActionList[*found]->GetType());
			LogIfNotNull("Performing action '" + state.GetName() + "'", index);
			func();
		}
		else if (!state.AcceptsTerminal(token.name))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				LogIfNotNull("Parser doesn't accept token '" +
					token.name + "' at error state", index);
				const std::string acceptables = string_utils::JoinStrings(
					*state.GetAcceptableTerminals(), ", ", "[", "]");
				LogIfNotNull("List of acceptable tokens: " + acceptables);
				return false;
			}
			else
			{
				LogIfNotNull("Parser doesn't accept token '" +
					token.name + "', switching to alternative #" +
					std::to_string(index + 1) + "...", index);
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
			try
			{
				token = FetchNextToken(index);
			}
			catch (const std::exception& ex)
			{
				LogIfNotNull("Lexer error: "s + ex.what(), index);
				return false;
			}
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
	return *mTable;
}

void Parser::SetActionNames(const std::vector<std::string>& actions)
{
	for (const std::string& name : actions)
	{
		mActionList.emplace_back(std::make_unique<Action>(name, ActionType::None));
	}
}

void Parser::SetAction(size_t index, std::unique_ptr<IAction> && action)
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	mActionList[index] = std::move(action);
}

void Parser::SwapActions(size_t oldPos, size_t newPos)
{
	std::iter_swap(mActionList.begin() + oldPos, mActionList.begin() + newPos);
}

const IAction& Parser::GetAction(size_t index) const
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *mActionList[index];
}

IAction& Parser::GetAction(size_t index)
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *mActionList[index];
}

size_t Parser::GetActionsCount() const
{
	return mActionList.size();
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

Token Parser::FetchNextToken(size_t currentStateIndex)
{
	const Token token = mLexer.GetNextToken();
	const auto fmt = boost::format("Token (%1%%2%) read by lexer")
		% token.name
		% (token.value != token.name && !token.value.empty() ? (", \"" + token.value + "\"") : "");
	LogIfNotNull(fmt.str(), currentStateIndex);
	return token;
}

boost::optional<size_t> Parser::FindActionIndexByName(const std::string& name)
{
	for (size_t i = 0; i < mActionList.size(); ++i)
	{
		const auto& pAction = mActionList[i];
		if (pAction->GetName() == name)
		{
			return boost::make_optional(i);
		}
	}
	return boost::none;
}

void Parser::LogIfNotNull(
	const std::string& message,
	boost::optional<size_t> state,
	bool newline
)
{
	if (!mLogger)
	{
		return;
	}

	mLogger->Log((state ? "[#" + std::to_string(*state) + "] " : "") + message);
	if (newline)
	{
		mLogger->Log("\n");
	}
}
