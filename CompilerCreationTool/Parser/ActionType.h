#pragma once
#include <vector>
#include <string>

enum class ActionType
{
	None,
	// Expressions
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
	CreateStringLiteralNode,
	// Types
	CreateIntTypeNode,
	CreateFloatTypeNode,
	CreateBoolTypeNode,
	CreateStringTypeNode,
	// Statements
	CreateVariableDeclarationNode,
	SaveOptionalAssignExpression,
	CreateAssignNode,
	CreateIfStatementNode,
	SaveOptionalElseStatement,
	CreateWhileStatementNode,
	CreatePrintStatementNode,
	AllocateNewComposite,
	OnCompositePartParse,
	MoveCompositeToStatement
};

std::vector<ActionType> const& GetActionTypesList();
int GetActionTypeIndex(ActionType type);

std::string ToString(ActionType type);
std::string ToPrettyString(ActionType type);
std::string GetDescription(ActionType type);
ActionType ToActionType(const std::string& str);
