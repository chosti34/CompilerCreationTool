#pragma once
#include "../AST/AST.h"
#include "../AST/NodeVisitor.h"
#include <fstream>

class ASTGraphvizVisualizer : public INodeVisitor
{
public:
	explicit ASTGraphvizVisualizer(const std::string& filepath);
	~ASTGraphvizVisualizer();

	void Visualize(const IExpressionAST& expression);

private:
	void Visit(const LiteralExpressionAST& literal) override;
	void Visit(const BinaryExpressionAST& binary) override;

private:
	std::ofstream mOutput;
	size_t mIndex = 0;
	std::vector<size_t> mChildren;
};
