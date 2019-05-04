#pragma once
#include "NodeVisitor.h"
#include <boost/variant.hpp>
#include <string>

class IExpressionAST
{
public:
	virtual ~IExpressionAST() = default;
	virtual void Accept(IExpressionNodeVisitor& visitor) const = 0;
};

class LiteralExpressionAST : public IExpressionAST
{
public:
	using Value = boost::variant<int, double, bool, std::string>;

	explicit LiteralExpressionAST(const Value& value)
		: mValue(value)
	{
	}

	const Value& GetValue() const
	{
		return mValue;
	}

	void Accept(IExpressionNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	Value mValue;
};

class BinaryExpressionAST : public IExpressionAST
{
public:
	enum Operator
	{
		Plus,
		Minus,
		Mul,
		Div,
		Less,
		Or,
		And
	};

	explicit BinaryExpressionAST(
		std::unique_ptr<IExpressionAST> && left,
		std::unique_ptr<IExpressionAST> && right,
		Operator op
	)
		: mLeft(std::move(left))
		, mRight(std::move(right))
		, mOperator(op)
	{
	}

	const IExpressionAST& GetLeft() const
	{
		return *mLeft;
	}

	const IExpressionAST& GetRight() const
	{
		return *mRight;
	}

	Operator GetOperator() const
	{
		return mOperator;
	}

	void Accept(IExpressionNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mLeft;
	std::unique_ptr<IExpressionAST> mRight;
	Operator mOperator;
};

class UnaryExpressionAST : public IExpressionAST
{
public:
	enum Operator
	{
		Plus,
		Minus,
		Negation
	};

	explicit UnaryExpressionAST(std::unique_ptr<IExpressionAST> && expression, Operator op)
		: mExpression(std::move(expression))
		, mOperator(op)
	{
	}

	const IExpressionAST& GetExpression() const
	{
		return *mExpression;
	}

	Operator GetOperator() const
	{
		return mOperator;
	}

	void Accept(IExpressionNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mExpression;
	Operator mOperator;
};

class IdentifierExpressionAST : public IExpressionAST
{
public:
	explicit IdentifierExpressionAST(const std::string& name)
		: mName(name)
	{
	}

	const std::string& GetName() const
	{
		return mName;
	}

	void Accept(IExpressionNodeVisitor& visitor) const
	{
		visitor.Visit(*this);
	}

private:
	std::string mName;
};

inline std::string ToString(BinaryExpressionAST::Operator op)
{
	switch (op)
	{
	case BinaryExpressionAST::Plus:
		return "+";
	case BinaryExpressionAST::Minus:
		return "-";
	case BinaryExpressionAST::Mul:
		return "*";
	case BinaryExpressionAST::Div:
		return "/";
	case BinaryExpressionAST::Less:
		return "<";
	case BinaryExpressionAST::Or:
		return "||";
	case BinaryExpressionAST::And:
		return "&&";
	default:
		assert(false);
		throw std::logic_error("can't cast undefined binary expression operator to string");
	}
}

inline std::string ToString(UnaryExpressionAST::Operator op)
{
	switch (op)
	{
	case UnaryExpressionAST::Plus:
		return "+";
	case UnaryExpressionAST::Minus:
		return "-";
	case UnaryExpressionAST::Negation:
		return "!";
	default:
		assert(false);
		throw std::logic_error("can't cast undefined unary expression operator to string");
	}
}
