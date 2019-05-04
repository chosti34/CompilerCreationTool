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
