#pragma once
#include "IParser.h"
#include "ParseResults.h"
#include "IParserTable.h"
#include "IParserLogger.h"
#include "../Lexer/ILexer.h"

class Parser : public IParser<ParseResults>
{
public:
	using ActionPtrList = std::vector<std::unique_ptr<IAction>>;
	using ParserLoggerPtr = std::unique_ptr<IParserLogger>;
	using ParserTablePtr = std::unique_ptr<IParserTable>;

public:
	explicit Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer);

	ParseResults Parse(const std::string& text) override;
	const IParserTable& GetTable() const override;

	void SetActionNames(const std::vector<std::string>& actions) override;
	void SetAction(size_t index, std::unique_ptr<IAction> && action) override;
	void SwapActions(size_t oldPos, size_t newPos) override;

	const IAction& GetAction(size_t index) const override;
	IAction& GetAction(size_t index) override;
	size_t GetActionsCount() const override;

	void SetLogger(std::unique_ptr<IParserLogger> && logger) override;
	const IParserLogger* GetLogger() const override;
	IParserLogger* GetLogger() override;

private:
	Token FetchNextToken(size_t currentStateIndex);
	boost::optional<size_t> FindActionIndexByName(const std::string& name);

	void LogIfNotNull(
		const std::string& message,
		boost::optional<size_t> state = boost::none,
		bool newline = true
	);

private:
	ParserTablePtr mTable;
	ILexer& mLexer;
	ActionPtrList mActionList;
	ParserLoggerPtr mLogger;
};
