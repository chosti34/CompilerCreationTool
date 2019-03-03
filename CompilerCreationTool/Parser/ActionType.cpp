#include "stdafx.h"
#include "ActionType.h"
#include <unordered_map>
#include <cassert>

namespace
{
const std::vector<ActionType> gcActionTypes = {
	ActionType::None,
	ActionType::PrintHello,
	ActionType::PrintWorld
};

const std::unordered_map<ActionType, std::string> gcActionStringMap = {
	{ ActionType::None, "None" },
	{ ActionType::PrintHello, "PrintHello" },
	{ ActionType::PrintWorld, "PrintWorld" }
};
}

const std::vector<ActionType>& GetAllActionTypes()
{
	return gcActionTypes;
}

int GetActionTypeIndex(ActionType type)
{
	for (int i = 0; i < gcActionTypes.size(); ++i)
	{
		if (gcActionTypes[i] == type)
		{
			return i;
		}
	}
	assert(false);
	throw std::logic_error("can't get index of undefined action type");
}

std::string ToString(ActionType type)
{
	auto found = gcActionStringMap.find(type);
	if (found != gcActionStringMap.end())
	{
		return found->second;
	}
	assert(false);
	throw std::logic_error("can't get string representation of undefined action type");
}
