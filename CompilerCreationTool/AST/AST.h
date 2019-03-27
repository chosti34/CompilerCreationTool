#pragma once
#include "NodeVisitor.h"
#include <boost/variant.hpp>
#include <string>

class IExpressionAST
{
public:
	virtual ~IExpressionAST() = default;
	virtual void Accept(INodeVisitor& visitor) const = 0;
};

class LiteralExpressionAST : public IExpressionAST
{
public:
	using Value = boost::variant<int, double>;

	explicit LiteralExpressionAST(const Value& value)
		: mValue(value)
	{
	}

	const Value& GetValue() const
	{
		return mValue;
	}

	void Accept(INodeVisitor& visitor) const override
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
		Div
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

	void Accept(INodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mLeft;
	std::unique_ptr<IExpressionAST> mRight;
	Operator mOperator;
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
	default:
		assert(false);
		throw std::logic_error("can't cast undefined binary expression operator to string");
	}
}
