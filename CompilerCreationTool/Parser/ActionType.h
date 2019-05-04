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
	CreateBinaryDivNode,
	CreateBinaryLessNode,
	CreateBinaryOrNode,
	CreateBinaryAndNode,
	CreateUnaryMinusNode,
	CreateUnaryPlusNode,
	CreateUnaryNegationNode,
	CreateIdentifier,
	CreateTrueNode,
	CreateFalseNode,
	CreateStringLiteralNode
};

std::vector<ActionType> const& GetActionTypesList();
int GetActionTypeIndex(ActionType type);

std::string ToString(ActionType type);
std::string ToPrettyString(ActionType type);
std::string GetDescription(ActionType type);
ActionType ToActionType(const std::string& str);
