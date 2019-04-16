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

const std::unordered_map<ActionType, std::string> gcActionPrettyStringMap = {
	{ ActionType::None, "None" },
	{ ActionType::CreateIntegerNumberNode, "Create integer number node" },
	{ ActionType::CreateFloatNumberNode, "Create float number node" },
	{ ActionType::CreateBinaryPlusNode, "Create binary plus node" },
	{ ActionType::CreateBinaryMinusNode, "Create binary minus node" },
	{ ActionType::CreateBinaryMulNode, "Create binary mul node" },
	{ ActionType::CreateBinaryDivNode, "Create binary div node" }
};

const std::unordered_map<ActionType, std::string> gcActionDescriptionMap = {
	{ ActionType::None, "Do nothing with the stack" },
	{ ActionType::CreateIntegerNumberNode, "Create integer number node from current token and push it to the expression stack" },
	{ ActionType::CreateFloatNumberNode, "Create float number node from current token and push it to the expression stack" },
	{ ActionType::CreateBinaryPlusNode, "Pop two last expressions from the expression stack, create binary plus node from them, then push it to the stack" },
	{ ActionType::CreateBinaryMinusNode, "Pop two last expressions from the expression stack, create binary minus node from them, then push it to the stack" },
	{ ActionType::CreateBinaryMulNode, "Pop two last expressions from the expression stack, create binary mul node from them, then push it to the stack" },
	{ ActionType::CreateBinaryDivNode, "Pop two last expressions from the expression stack, create binary div node from them, then push it to the stack" }
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

std::string ToPrettyString(ActionType type)
{
	auto found = gcActionPrettyStringMap.find(type);
	if (found != gcActionPrettyStringMap.end())
	{
		return found->second;
	}
	assert(false);
	throw std::logic_error("can't get pretty string of undefined action type");
}

std::string GetDescription(ActionType type)
{
	auto found = gcActionDescriptionMap.find(type);
	if (found != gcActionDescriptionMap.end())
	{
		return found->second;
	}
	assert(false);
	throw std::logic_error("can't get description of undefined action type");
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
