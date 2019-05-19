#pragma once
#include "../AST/AST.h"
#include "CodegenContext.h"
#include "ICodegenLogger.h"


class ExpressionCodegen : public IExpressionNodeVisitor
{
public:
	explicit ExpressionCodegen(CodegenContext& context, ICodegenLogger* logger = nullptr);
	llvm::Value* Visit(const IExpressionAST& node);

private:
	void Visit(const BinaryExpressionAST& binary) override;
	void Visit(const LiteralExpressionAST& literal) override;
	void Visit(const UnaryExpressionAST& unary) override;
	void Visit(const IdentifierExpressionAST& identifier) override;

private:
	CodegenContext& mContext;
	std::vector<llvm::Value*> mStack;
	ICodegenLogger* mLogger;
};


class StatementCodegen : public IStatementNodeVisitor
{
public:
	explicit StatementCodegen(CodegenContext& context, ICodegenLogger* logger = nullptr);
	void Visit(const IStatementAST& node);

private:
	void Visit(const VariableDeclarationAST& node) override;
	void Visit(const AssignStatementAST& node) override;
	void Visit(const IfStatementAST& node) override;
	void Visit(const WhileStatementAST& node) override;
	void Visit(const CompositeStatementAST& composite) override;
	void Visit(const PrintStatementAST& print) override;

private:
	CodegenContext& mContext;
	ExpressionCodegen mExpressionCodegen;
	std::vector<llvm::BasicBlock*> mBranchContinueStack;
	ICodegenLogger* mLogger; // can be nullptr
};


class Codegen
{
public:
	explicit Codegen(CodegenContext& context, std::unique_ptr<ICodegenLogger> && logger = nullptr);

	void Generate(const IStatementAST& statement);
	void Generate(const IExpressionAST& expression);

private:
	CodegenContext& mContext;
	std::unique_ptr<ICodegenLogger> mLogger; // can be nullptr
};
