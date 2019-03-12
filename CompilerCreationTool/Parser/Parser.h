#pragma once
#include "IParser.h"
#include "IParserTable.h"
#include "IParserLogger.h"
#include "../Lexer/ILexer.h"

class Parser : public IParser<bool>
{
public:
	explicit Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer);

	bool Parse(const std::string& text) override;
	const IParserTable& GetTable() const override;

	void SetActionNames(const std::vector<std::string> &actions) override;
	void SetAction(size_t index, std::unique_ptr<IAction> && action) override;
	void SwapActions(size_t oldPos, size_t newPos) override;

	const IAction& GetAction(size_t index) const override;
	IAction& GetAction(size_t index) override;
	size_t GetActionsCount() const override;

	void SetLogger(std::unique_ptr<IParserLogger> && logger) override;
	const IParserLogger* GetLogger() const override;
	IParserLogger* GetLogger() override;

private:
	boost::optional<size_t> FindActionIndexByName(const std::string& name);

private:
	std::vector<std::unique_ptr<IAction>> m_actions;
	std::unique_ptr<IParserLogger> mLogger;
	std::unique_ptr<IParserTable> m_table;
	ILexer& m_lexer;
};
