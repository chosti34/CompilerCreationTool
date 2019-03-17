#pragma once

class LiteralExpressionAST;
class BinaryExpressionAST;

class INodeVisitor
{
public:
	virtual ~INodeVisitor() = default;
	virtual void Visit(const LiteralExpressionAST& literal) = 0;
	virtual void Visit(const BinaryExpressionAST& binary) = 0;
};
