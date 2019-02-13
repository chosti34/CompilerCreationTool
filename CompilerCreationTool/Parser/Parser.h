#pragma once
#include "IParser.h"
#include "IParserTable.h"

class Parser : public IParser<bool>
{
public:
	explicit Parser(std::unique_ptr<IParserTable> && table);

	bool Parse(const std::string& text) override;

private:
	std::unique_ptr<IParserTable> m_table;
};
