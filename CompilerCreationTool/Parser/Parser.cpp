#include "stdafx.h"
#include "Parser.h"
#include "ParserState.h"

Parser::Parser(std::unique_ptr<IParserTable> && table)
	: m_table(std::move(table))
{
}

bool Parser::Parse(const std::string& text)
{
	size_t index = 1;
	while (true)
	{
		const IParserState& state = m_table->GetState(index);
	}
	return false;
}
