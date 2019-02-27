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

	void SetActionNames(const std::vector<std::string> &actions) override;
	void SetAction(size_t index, std::unique_ptr<IAction> && action) override;
	virtual const IAction& GetAction(size_t index) const override;
	virtual size_t GetActionsCount() const override;

private:
	std::vector<std::unique_ptr<IAction>> m_actions;
	std::unique_ptr<IParserTable> m_table;
	ILexer& m_lexer;
};
