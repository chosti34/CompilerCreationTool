#include "pch.h"
#include "ASTGraphvizVisualizer.h"
#include "../Utils/string_utils.h"

namespace
{
std::string ToString(const boost::variant<int, double> &variant)
{
	if (variant.type() == typeid(int))
	{
		return std::to_string(boost::get<int>(variant));
	}
	else if (variant.type() == typeid(double))
	{
		return string_utils::TrimTrailingZerosAndPeriod(boost::get<double>(variant));
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
	mOutput << "shape=\"circle\" label=\"Literal (" + ToString(literal.GetValue()) + ")\"];" << std::endl;
	mChildren.push_back(mIndex++);
}

void ASTGraphvizVisualizer::Visit(const BinaryExpressionAST& binary)
{
	const size_t parent = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Binary (" << ToString(binary.GetOperator()) << ")\"];" << std::endl;

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
