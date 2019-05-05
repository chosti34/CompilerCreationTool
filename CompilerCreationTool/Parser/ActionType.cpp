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
	ActionType::CreateBinaryDivNode,
	ActionType::CreateBinaryLessNode,
	ActionType::CreateBinaryOrNode,
	ActionType::CreateBinaryAndNode,
	ActionType::CreateUnaryMinusNode,
	ActionType::CreateUnaryPlusNode,
	ActionType::CreateUnaryNegationNode,
	ActionType::CreateIdentifier,
	ActionType::CreateTrueNode,
	ActionType::CreateFalseNode,
	ActionType::CreateStringLiteralNode,

	ActionType::CreateIntTypeNode,
	ActionType::CreateFloatTypeNode,
	ActionType::CreateBoolTypeNode,
	ActionType::CreateStringTypeNode,

	ActionType::CreateVariableDeclarationNode,
	ActionType::SaveOptionalAssignExpression,
	ActionType::CreateAssignNode,
	ActionType::CreateIfStatementNode,
	ActionType::SaveOptionalElseStatement,
};

const std::unordered_map<ActionType, std::string> gcActionStringMap = {
	{ ActionType::None, "None" },
	{ ActionType::CreateIntegerNumberNode, "CreateIntegerNumberNode" },
	{ ActionType::CreateFloatNumberNode, "CreateFloatNumberNode" },
	{ ActionType::CreateBinaryPlusNode, "CreateBinaryPlusNode" },
	{ ActionType::CreateBinaryMinusNode, "CreateBinaryMinusNode" },
	{ ActionType::CreateBinaryMulNode, "CreateBinaryMulNode" },
	{ ActionType::CreateBinaryDivNode, "CreateBinaryDivNode" },
	{ ActionType::CreateBinaryLessNode, "CreateBinaryLessNode" },
	{ ActionType::CreateBinaryOrNode, "CreateBinaryOrNode" },
	{ ActionType::CreateBinaryAndNode, "CreateBinaryAndNode" },
	{ ActionType::CreateUnaryMinusNode, "CreateUnaryMinusNode" },
	{ ActionType::CreateUnaryPlusNode, "CreateUnaryPlusNode" },
	{ ActionType::CreateUnaryNegationNode, "CreateUnaryNegationNode" },
	{ ActionType::CreateIdentifier, "CreateIdentifier" },
	{ ActionType::CreateTrueNode, "CreateTrueNode" },
	{ ActionType::CreateFalseNode, "CreateFalseNode" },
	{ ActionType::CreateStringLiteralNode, "CreateStringLiteralNode" },

	{ ActionType::CreateIntTypeNode, "CreateIntTypeNode" },
	{ ActionType::CreateFloatTypeNode, "CreateFloatTypeNode" },
	{ ActionType::CreateBoolTypeNode, "CreateBoolTypeNode" },
	{ ActionType::CreateStringTypeNode, "CreateStringTypeNode" },

	{ ActionType::CreateVariableDeclarationNode, "CreateVariableDeclarationNode" },
	{ ActionType::SaveOptionalAssignExpression, "SaveOptionalAssignExpression" },
	{ ActionType::CreateAssignNode, "CreateAssignNode" },
	{ ActionType::CreateIfStatementNode, "CreateIfStatementNode" },
	{ ActionType::SaveOptionalElseStatement, "SaveOptionalElseStatement" },
};

const std::unordered_map<ActionType, std::string> gcActionPrettyStringMap = {
	{ ActionType::None, "None" },
	{ ActionType::CreateIntegerNumberNode, "Create integer number node" },
	{ ActionType::CreateFloatNumberNode, "Create float number node" },
	{ ActionType::CreateBinaryPlusNode, "Create binary plus node" },
	{ ActionType::CreateBinaryMinusNode, "Create binary minus node" },
	{ ActionType::CreateBinaryMulNode, "Create binary mul node" },
	{ ActionType::CreateBinaryDivNode, "Create binary div node" },
	{ ActionType::CreateBinaryLessNode, "Create binary less node" },
	{ ActionType::CreateBinaryOrNode, "Create binary or node" },
	{ ActionType::CreateBinaryAndNode, "Create binary and node" },
	{ ActionType::CreateUnaryMinusNode, "Create unary minus node" },
	{ ActionType::CreateUnaryPlusNode, "Create unary plus node" },
	{ ActionType::CreateUnaryNegationNode, "Create unary negation node" },
	{ ActionType::CreateIdentifier, "Create identifier node" },
	{ ActionType::CreateTrueNode, "Create boolean true node" },
	{ ActionType::CreateFalseNode, "Create boolean false node" },
	{ ActionType::CreateStringLiteralNode, "Create string literal node" },

	{ ActionType::CreateIntTypeNode, "Create int type node" },
	{ ActionType::CreateFloatTypeNode, "Create float type node" },
	{ ActionType::CreateBoolTypeNode, "Create bool type node" },
	{ ActionType::CreateStringTypeNode, "Create string type node" },

	{ ActionType::CreateVariableDeclarationNode, "Create variable declaration node" },
	{ ActionType::SaveOptionalAssignExpression, "Save optional assign expression" },
	{ ActionType::CreateAssignNode, "Create assign node" },
	{ ActionType::CreateIfStatementNode, "Create if statement node" },
	{ ActionType::SaveOptionalElseStatement, "Save optional else statement" },
};

const std::unordered_map<ActionType, std::string> gcActionDescriptionMap = {
	{ ActionType::None, "Do nothing with the stack" },
	{ ActionType::CreateIntegerNumberNode, "Create integer number node from current token and push it to the expression stack" },
	{ ActionType::CreateFloatNumberNode, "Create float number node from current token and push it to the expression stack" },
	{ ActionType::CreateBinaryPlusNode, "Pop two last expressions from the expression stack, create binary plus node from them, then push it to the stack" },
	{ ActionType::CreateBinaryMinusNode, "Pop two last expressions from the expression stack, create binary minus node from them, then push it to the stack" },
	{ ActionType::CreateBinaryMulNode, "Pop two last expressions from the expression stack, create binary mul node from them, then push it to the stack" },
	{ ActionType::CreateBinaryDivNode, "Pop two last expressions from the expression stack, create binary div node from them, then push it to the stack" },
	{ ActionType::CreateBinaryLessNode, "Pop two last expressions from the expression stack, create binary less node from them, then push it to the stack" },
	{ ActionType::CreateBinaryOrNode, "Pop two last expressions from the expression stack, create binary or node from them, then push it to the stack" },
	{ ActionType::CreateBinaryAndNode, "Pop two last expressions from the expression stack, create binary and node from them, then push it to the stack" },
	{ ActionType::CreateUnaryMinusNode, "Pops expression from the stack, create unary minus expression and push it to the stack" },
	{ ActionType::CreateUnaryPlusNode, "Pops expression from the stack, create unary plus expression and push it to the stack" },
	{ ActionType::CreateUnaryNegationNode, "Pops expression from the stack, create unary negation expression and push it to the stack" },
	{ ActionType::CreateIdentifier, "Create identifier node with name of current token, then push it to the stack" },
	{ ActionType::CreateTrueNode, "Create boolean true node and push it to the stack" },
	{ ActionType::CreateFalseNode, "Create boolean false node and push it to the stack" },
	{ ActionType::CreateStringLiteralNode, "Create string literal node from current token, then push it to the stack" },

	{ ActionType::CreateIntTypeNode, "Create int type node and push it to the type stack" },
	{ ActionType::CreateFloatTypeNode, "Create float type node and push it to the stack" },
	{ ActionType::CreateBoolTypeNode, "Create bool type node and push it to the stack" },
	{ ActionType::CreateStringTypeNode, "Create string type node and push it to the stack" },

	{ ActionType::CreateVariableDeclarationNode, "Pop identifier, type, and optional assign expression (if created), create declaration statement and push it to the stack" },
	{ ActionType::SaveOptionalAssignExpression, "Save last created expression for declaration optional assign" },
	{ ActionType::CreateAssignNode, "Create assign node and push it to the stack" },
	{ ActionType::CreateIfStatementNode, "Pop then statement, else clause statement (if created) and expression, then create if statement node from them, and push it to the stack" },
	{ ActionType::SaveOptionalElseStatement, "Pop if statement from the stack, attach else clause to it and push it back" },
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
