#pragma once
#include "IParserTable.h"
#include "IParserLogger.h"
#include "IAction.h"

template <typename T = void>
class IParser
{
public:
	virtual ~IParser() = default;

	virtual T Parse(const std::string& text) = 0;
	virtual const IParserTable& GetTable() const = 0;

	virtual void SetActionNames(const std::vector<std::string> &actions) = 0;
	virtual void SetAction(size_t index, std::unique_ptr<IAction> && action) = 0;
	virtual void SetActions(std::vector<std::unique_ptr<IAction>> && actions) = 0;
	virtual void SwapActions(size_t lhs, size_t rhs) = 0;

	virtual boost::optional<size_t> GetActionPos(const std::string& name) const = 0;
	virtual const IAction& GetAction(size_t index) const = 0;
	virtual IAction& GetAction(size_t index) = 0;
	virtual size_t GetActionsCount() const = 0;

	virtual void SetLogger(std::unique_ptr<IParserLogger> && logger) = 0;
	virtual const IParserLogger* GetLogger() const = 0;
	virtual IParserLogger* GetLogger() = 0;
};
