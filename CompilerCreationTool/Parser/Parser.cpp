#include "stdafx.h"
#include "Parser.h"
#include "ParserState.h"
#include <iostream>
#include <sstream>

Parser::Parser(std::unique_ptr<IParserTable> && table)
	: m_table(std::move(table))
{
}

bool Parser::Parse(const std::string& text)
{
	std::istringstream strm(text);
	std::string token;
	strm >> token;

	size_t index = 0;
	std::vector<size_t> addresses;

	while (true)
	{
		const IParserState& state = m_table->GetState(index);

		if (state.GetFlag(StateFlag::Attribute))
		{
			std::cout << "Action " << state.GetName() << std::endl;
		}
		else if (!state.AcceptsTerminal(token))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				std::cout << "Unexpected token " << token << std::endl;
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
			strm >> token;
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
	return false;
}
