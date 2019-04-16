#pragma once
#include "ActionType.h"
#include <boost/optional.hpp>
#include <string>

class IAction
{
public:
	virtual ~IAction() = default;

	virtual void SetName(const std::string& name) = 0;
	virtual const std::string& GetName() const = 0;

	virtual void SetType(ActionType type) = 0;
	virtual ActionType GetType() const = 0;

	virtual void SetMessage(std::string message) = 0;
	virtual const std::string& GetMessage() const = 0;
};
