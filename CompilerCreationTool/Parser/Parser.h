#pragma once
#include "IParser.h"
#include "IParserTable.h"
#include "../Lexer/ILexer.h"

class Parser : public IParser<bool>
{
public:
	explicit Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer);

	bool Parse(const std::string& text) override;
	const IParserTable& GetTable() const override;

private:
	std::unique_ptr<IParserTable> m_table;
	ILexer& m_lexer;
};
