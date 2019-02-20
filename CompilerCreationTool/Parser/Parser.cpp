#include "stdafx.h"
#include "Parser.h"
#include "ParserState.h"
#include <iostream>
#include <sstream>

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
			// std::cout << "Action " << state.GetName() << std::endl;
		}
		else if (!state.AcceptsTerminal(token.name))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				// std::cout << "Unexpected token " << token << std::endl;
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
	return false;
}

const IParserTable& Parser::GetTable() const
{
	return *m_table;
}
