#include "pch.h"
#include "ASTGraphvizVisualizer.h"
#include "../Utils/string_utils.h"

using namespace std::literals::string_literals;

namespace
{
std::string ToString(const LiteralExpressionAST::Value& variant)
{
	if (variant.type() == typeid(int))
	{
		return "Int: "s + std::to_string(boost::get<int>(variant));
	}
	else if (variant.type() == typeid(double))
	{
		return "Float: "s + string_utils::TrimTrailingZerosAndPeriod(boost::get<double>(variant));
	}
	else if (variant.type() == typeid(bool))
	{
		return "Boolean: "s + (boost::get<bool>(variant) ? "true"s : "false"s);
	}
	else if (variant.type() == typeid(std::string))
	{
		std::string str = boost::get<std::string>(variant);
		assert(str.size() >= 2);
		return "String: "s + str.substr(1, str.size() - 2);
	}
	assert(false);
	throw std::logic_error("can't get string representation of undefined variant");
}
}

ASTGraphvizExpressionVisualizer::ASTGraphvizExpressionVisualizer(std::ostream& os, size_t& index)
	: mOutput(os)
	, mIndex(index)
{
}

void ASTGraphvizExpressionVisualizer::Visualize(const IExpressionAST& expression)
{
	expression.Accept(*this);
}

void ASTGraphvizExpressionVisualizer::Visit(const LiteralExpressionAST& literal)
{
	mOutput << mIndex << " [";
	mOutput << "shape=\"circle\" label=\"" + ToString(literal.GetValue()) + "\"];" << std::endl;
	mChildren.push_back(mIndex++);
}

void ASTGraphvizExpressionVisualizer::Visit(const BinaryExpressionAST& binary)
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

void ASTGraphvizExpressionVisualizer::Visit(const UnaryExpressionAST& unary)
{
	const size_t that = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Unary (" << ToString(unary.GetOperator()) << ")\"];" << std::endl;

	unary.GetExpression().Accept(*this);

	size_t child = mChildren.back();
	mChildren.pop_back();

	mOutput << that << "->" << child << " [label=\" \"];" << std::endl;
	mChildren.push_back(that);
}

void ASTGraphvizExpressionVisualizer::Visit(const IdentifierExpressionAST& identifier)
{
	mOutput << mIndex << " [";
	mOutput << "shape=\"circle\" label=\"Identifier (" << identifier.GetName() << ")\"];" << std::endl;
	mChildren.push_back(mIndex++);
}


ASTGraphvizStatementVisualizer::ASTGraphvizStatementVisualizer(std::ostream& os, size_t& index)
	: mOutput(os)
	, mIndex(index)
{
}

void ASTGraphvizStatementVisualizer::Visualize(const IStatementAST& statement)
{
	statement.Accept(*this);
}

void ASTGraphvizStatementVisualizer::Visit(const VariableDeclarationAST& declaration)
{
	const size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Declaration (";
	mOutput << declaration.GetIdentifier().GetName() << ", " << ToString(declaration.GetType()) << ")\"];" << std::endl;

	if (const IExpressionAST* expression = declaration.GetExpression())
	{
		const size_t expressionIndex = mIndex;
		ASTGraphvizExpressionVisualizer expressionVisualizer(mOutput, mIndex);
		expressionVisualizer.Visualize(*expression);

		mOutput << statementIndex << "->" << expressionIndex << " [label=\" \"];" << std::endl;
	}
}

void ASTGraphvizStatementVisualizer::Visit(const AssignStatementAST& assign)
{
	const size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Assign\"]" << std::endl;

	const size_t identifierIndex = mIndex;
	ASTGraphvizExpressionVisualizer expressionVisualizer(mOutput, mIndex);
	expressionVisualizer.Visualize(assign.GetIdentifier());
	mOutput << statementIndex << "->" << identifierIndex << " [label=\" \"];" << std::endl;

	const std::size_t expressionIndex = mIndex;
	expressionVisualizer.Visualize(assign.GetExpression());

	mOutput << statementIndex << "->" << expressionIndex << " [label=\" \"];" << std::endl;
}

void ASTGraphvizStatementVisualizer::Visit(const IfStatementAST& ifStatement)
{
	const std::size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"If\"]" << std::endl;

	const size_t expressionIndex = mIndex;
	ASTGraphvizExpressionVisualizer expressionVisualizer(mOutput, mIndex);
	expressionVisualizer.Visualize(ifStatement.GetExpression());
	mOutput << statementIndex << "->" << expressionIndex << " [label=\" condition\"];" << std::endl;

	const size_t thenIndex = mIndex;
	ifStatement.GetThenStatement().Accept(*this);
	mOutput << statementIndex << "->" << thenIndex << " [label=\" then\"];" << std::endl;

	const size_t elseIndex = mIndex;
	if (const IStatementAST* elseStatement = ifStatement.GetElseStatement())
	{
		elseStatement->Accept(*this);
		mOutput << statementIndex << "->" << elseIndex << " [label=\" else\"];" << std::endl;
	}
}

void ASTGraphvizStatementVisualizer::Visit(const WhileStatementAST& whileStatement)
{
	const std::size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"While\"]" << std::endl;

	const std::size_t expressionIndex = mIndex;
	ASTGraphvizExpressionVisualizer expressionVisualizer(mOutput, mIndex);
	expressionVisualizer.Visualize(whileStatement.GetExpression());
	mOutput << statementIndex << "->" << expressionIndex << " [label=\" condition\"];" << std::endl;

	const std::size_t bodyIndex = mIndex;
	whileStatement.GetStatement().Accept(*this);
	mOutput << statementIndex << "->" << bodyIndex << " [label=\" body\"];" << std::endl;
}

void ASTGraphvizStatementVisualizer::Visit(const PrintStatementAST& print)
{
	const std::size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Print\"]" << std::endl;

	const std::size_t expressionIndex = mIndex;
	ASTGraphvizExpressionVisualizer expressionVisualizer(mOutput, mIndex);
	expressionVisualizer.Visualize(print.GetExpression());
	mOutput << statementIndex << "->" << expressionIndex << " [label=\" \"];" << std::endl;
}

void ASTGraphvizStatementVisualizer::Visit(const CompositeStatementAST& composite)
{
	const std::size_t statementIndex = mIndex;

	mOutput << mIndex++ << " [";
	mOutput << "shape=\"circle\" label=\"Statement List\"]" << std::endl;

	for (std::size_t i = 0; i < composite.GetStatementsCount(); ++i)
	{
		const std::size_t currentIndex = mIndex;
		composite.GetStatement(i).Accept(*this);
		mOutput << statementIndex << "->" << currentIndex << " [label=\" \"];" << std::endl;
	}
}
