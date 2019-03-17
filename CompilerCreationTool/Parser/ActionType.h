#pragma once
#include <vector>
#include <string>

enum class ActionType
{
	None,
	CreateIntegerNumberNode,
	CreateFloatNumberNode,
	CreateBinaryPlusNode,
	CreateBinaryMinusNode,
	CreateBinaryMulNode,
	CreateBinaryDivNode
};

const std::vector<ActionType>& GetActionTypesList();
int GetActionTypeIndex(ActionType type);
std::string ToString(ActionType type);
