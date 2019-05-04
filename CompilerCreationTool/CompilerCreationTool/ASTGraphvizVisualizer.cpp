#include "pch.h"
#include "ASTGraphvizVisualizer.h"
#include "../Utils/string_utils.h"

namespace
{
std::string ToString(const LiteralExpressionAST::Value& variant)
{
	if (variant.type() == typeid(int))
	{
		return "Int: " + std::to_string(boost::get<int>(variant));
	}
	else if (variant.type() == typeid(double))
	{
		return "Float: " + string_utils::TrimTrailingZerosAndPeriod(boost::get<double>(variant));
	}
	else if (variant.type() == typeid(bool))
	{
		return "Boolean: " + boost::get<bool>(variant) ? "true" : "false";
	}
	else if (variant.type() == typeid(std::string))
	{
		std::string str = boost::get<std::string>(variant);
		assert(str.size() >= 2);
		return "String: " + str.substr(1, str.size() - 2);
	}
	assert(false);
	throw std::logic_error("can't get string representation of undefined variant");
}
}

ASTGraphvizVisualizer::ASTGraphvizVisualizer(const std::string& filepath)
	: mOutput(filepath)
{
	mOutput << "digraph {" << std::endl;
}

ASTGraphvizVisualizer::~ASTGraphvizVisualizer()
{
	mOutput << "}" << std::endl;
}

void ASTGraphvizVisualizer::Visualize(const IExpressionAST& expression)
{
	expression.Accept(*this);
}

void ASTGraphvizVisualizer::Visit(const LiteralExpressionAST& literal)
{
	mOutput << mIndex << " [";
	mOutput << "shape=\"circle\" label=\"" + ToString(literal.GetValue()) + "\"];" << std::endl;
	mChildren.push_back(mIndex++);
}

void ASTGraphvizVisualizer::Visit(const BinaryExpressionAST& binary)
{
	const size_t parent = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"" << ToString(binary.GetOperator()) << "\"];" << std::endl;

	binary.GetLeft().Accept(*this);
	binary.GetRight().Accept(*this);

	size_t right = mChildren.back();
	mChildren.pop_back();

	size_t left = mChildren.back();
	mChildren.pop_back();

	mOutput << parent << "->" << left << " [label=\" \"];" << std::endl;
	mOutput << parent << "->" << right << " [label=\" \"];" << std::endl;
	mChildren.push_back(parent);
}

void ASTGraphvizVisualizer::Visit(const UnaryExpressionAST& unary)
{
	const size_t that = mIndex;

	mOutput << mIndex++ << "[";
	mOutput << "shape=\"circle\" label=\"Unary (" << ToString(unary.GetOperator()) << ")\"];" << std::endl;

	unary.GetExpression().Accept(*this);

	size_t child = mChildren.back();
	mChildren.pop_back();

	mOutput << that << "->" << child << " [label=\" \"];" << std::endl;
	mChildren.push_back(that);
}

void ASTGraphvizVisualizer::Visit(const IdentifierExpressionAST& identifier)
{
	mOutput << mIndex << "[";
	mOutput << "shape=\"circle\" label=\"Identifier (" << identifier.GetName() << ")\"];" << std::endl;
	mChildren.push_back(mIndex++);
}
