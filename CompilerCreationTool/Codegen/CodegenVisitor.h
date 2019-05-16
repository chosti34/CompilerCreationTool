#pragma once
#include "../AST/AST.h"
#include "CodegenContext.h"


class ExpressionCodegen : public IExpressionNodeVisitor
{
public:
	explicit ExpressionCodegen(CodegenContext& context);
	llvm::Value* Visit(const IExpressionAST& node);

private:
	void Visit(const BinaryExpressionAST& binary) override;
	void Visit(const LiteralExpressionAST& literal) override;
	void Visit(const UnaryExpressionAST& unary) override;
	void Visit(const IdentifierExpressionAST& identifier) override;

private:
	CodegenContext& m_context;
	std::vector<llvm::Value*> m_stack;
};


class StatementCodegen : public IStatementNodeVisitor
{
public:
	explicit StatementCodegen(CodegenContext& context);
	void Visit(const IStatementAST& node);

private:
	void Visit(const VariableDeclarationAST& node) override;
	void Visit(const AssignStatementAST& node) override;
	void Visit(const IfStatementAST& node) override;
	void Visit(const WhileStatementAST& node) override;
	void Visit(const CompositeStatementAST& composite) override;
	void Visit(const PrintStatementAST& print) override;

private:
	CodegenContext& m_context;
	ExpressionCodegen m_expressionCodegen;
	std::vector<llvm::BasicBlock*> m_branchContinueStack;
};


class Codegen
{
public:
	explicit Codegen(CodegenContext& context);

	void Generate(const IStatementAST& statement);
	void Generate(const IExpressionAST& expression);

private:
	CodegenContext& m_context;
};
