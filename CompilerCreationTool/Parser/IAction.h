#pragma once
#include "ActionType.h"
#include <string>

class IAction
{
public:
	virtual ~IAction() = default;
	virtual ActionType GetType() const = 0;
	virtual const std::string& GetName() const = 0;
};
