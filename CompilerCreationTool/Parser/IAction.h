#pragma once
#include "ActionType.h"
#include <string>

class IAction
{
public:
	virtual ~IAction() = default;
	virtual void SetName(const std::string& name) = 0;
	virtual const std::string& GetName() const = 0;
	virtual void SetType(ActionType type) = 0;
	virtual ActionType GetType() const = 0;
};
