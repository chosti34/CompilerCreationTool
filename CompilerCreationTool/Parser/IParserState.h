#pragma once
#include "StateFlag.h"
#include <boost/optional.hpp>
#include <string>
#include <set>

class IParserState
{
public:
	virtual ~IParserState() = default;

	virtual bool GetFlag(StateFlag flag) const = 0;
	virtual void SetFlag(StateFlag flag, bool value) = 0;

	virtual const std::string& GetName() const = 0;
	virtual void SetName(const std::string& name) = 0;

	virtual boost::optional<size_t> GetNextAddress() const = 0;
	virtual void SetNextAddress(boost::optional<size_t> address) = 0;

	virtual const std::set<std::string>* GetAcceptableTerminals() const = 0;
	virtual void SetAcceptableTerminals(const std::set<std::string>& terminals) = 0;
	virtual bool AcceptsTerminal(const std::string& terminal) const = 0;
};
