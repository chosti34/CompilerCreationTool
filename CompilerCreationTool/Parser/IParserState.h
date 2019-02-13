#pragma once
#include <set>
#include <string>
#include <boost/optional.hpp>

class IParserState
{
public:
	virtual ~IParserState() = default;

	virtual bool GetShiftFlag() const = 0;
	virtual void SetShiftFlag(bool shift) = 0;

	virtual bool GetPushFlag() const = 0;
	virtual void SetPushFlag(bool push) = 0;

	virtual bool GetErrorFlag() const = 0;
	virtual void SetErrorFlag(bool error) = 0;

	virtual bool GetEndFlag() const = 0;
	virtual void SetEndFlag(bool end) = 0;

	virtual const std::string& GetName() const = 0;
	virtual void SetName(const std::string& name) = 0;

	virtual boost::optional<size_t> GetNextAddress() const = 0;
	virtual void SetNextAddress(boost::optional<size_t> address) = 0;

	virtual const std::set<std::string>& GetAcceptableTerminals() const = 0;
	virtual void SetAcceptableTerminals(const std::set<std::string>& terminals) = 0;

	virtual bool AcceptsTerminal(const std::string& terminal) const = 0;
};
