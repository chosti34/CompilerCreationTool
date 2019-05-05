#pragma once
#include "../AST/AST.h"
#include "../AST/NodeVisitor.h"
#include <ostream>

class ASTGraphvizExpressionVisualizer : public IExpressionNodeVisitor
{
public:
	explicit ASTGraphvizExpressionVisualizer(std::ostream& os, size_t& index);
	void Visualize(const IExpressionAST& expression);

private:
	void Visit(const LiteralExpressionAST& literal) override;
	void Visit(const BinaryExpressionAST& binary) override;
	void Visit(const UnaryExpressionAST& unary) override;
	void Visit(const IdentifierExpressionAST& identifier) override;

private:
	std::ostream& mOutput;
	size_t& mIndex;
	std::vector<size_t> mChildren;
};


class ASTGraphvizStatementVisualizer : public IStatementNodeVisitor
{
public:
	explicit ASTGraphvizStatementVisualizer(std::ostream& os, size_t& index);
	void Visualize(const IStatementAST& statement);

private:
	void Visit(const VariableDeclarationAST& declaration) override;
	void Visit(const AssignStatementAST& assign) override;
	void Visit(const IfStatementAST& ifStatement) override;
	void Visit(const WhileStatementAST& whileStatement) override;
	void Visit(const PrintStatementAST& print) override;
	void Visit(const CompositeStatementAST& composite) override;

private:
	std::ostream& mOutput;
	std::size_t& mIndex;
	std::vector<std::size_t> mChildren;
};
