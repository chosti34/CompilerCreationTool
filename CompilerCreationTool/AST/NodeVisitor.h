#pragma once

class LiteralExpressionAST;
class BinaryExpressionAST;
class UnaryExpressionAST;
class IdentifierExpressionAST;

class IExpressionNodeVisitor
{
public:
	virtual ~IExpressionNodeVisitor() = default;
	virtual void Visit(const LiteralExpressionAST& literal) = 0;
	virtual void Visit(const BinaryExpressionAST& binary) = 0;
	virtual void Visit(const UnaryExpressionAST& unary) = 0;
	virtual void Visit(const IdentifierExpressionAST& identifier) = 0;
};

class VariableDeclarationAST;
class AssignStatementAST;
class IfStatementAST;
class WhileStatementAST;
class PrintStatementAST;
class CompositeStatementAST;

class IStatementNodeVisitor
{
public:
	virtual ~IStatementNodeVisitor() = default;
	virtual void Visit(const VariableDeclarationAST& declaration) = 0;
	virtual void Visit(const AssignStatementAST& assign) = 0;
	virtual void Visit(const IfStatementAST& ifStatement) = 0;
	virtual void Visit(const WhileStatementAST& whileStatement) = 0;
	virtual void Visit(const PrintStatementAST& print) = 0;
	virtual void Visit(const CompositeStatementAST& composite) = 0;
};
