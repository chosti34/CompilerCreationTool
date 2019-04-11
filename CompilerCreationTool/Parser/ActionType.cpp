#include "stdafx.h"
#include "ActionType.h"
#include <unordered_map>
#include <algorithm>
#include <cassert>

namespace
{
const std::vector<ActionType> gcActionTypes = {
	ActionType::None,
	ActionType::CreateIntegerNumberNode,
	ActionType::CreateFloatNumberNode,
	ActionType::CreateBinaryPlusNode,
	ActionType::CreateBinaryMinusNode,
	ActionType::CreateBinaryMulNode,
	ActionType::CreateBinaryDivNode
};

const std::unordered_map<ActionType, std::string> gcActionStringMap = {
	{ ActionType::None, "None" },
	{ ActionType::CreateIntegerNumberNode, "CreateIntegerNumberNode" },
	{ ActionType::CreateFloatNumberNode, "CreateFloatNumberNode" },
	{ ActionType::CreateBinaryPlusNode, "CreateBinaryPlusNode" },
	{ ActionType::CreateBinaryMinusNode, "CreateBinaryMinusNode" },
	{ ActionType::CreateBinaryMulNode, "CreateBinaryMulNode" },
	{ ActionType::CreateBinaryDivNode, "CreateBinaryDivNode" }
};
}

const std::vector<ActionType>& GetActionTypesList()
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

ActionType ToActionType(const std::string& str)
{
	auto found = std::find_if(gcActionStringMap.begin(), gcActionStringMap.end(), [&](std::pair<ActionType, std::string> && pair) {
		return pair.second == str;
	});

	if (found != gcActionStringMap.end())
	{
		return found->first;
	}

	throw std::runtime_error("can't convert string '" + str + "' to action type");
}
