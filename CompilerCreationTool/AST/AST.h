#pragma once
#include "NodeVisitor.h"
#include "ExpressionType.h"
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

class IStatementAST
{
public:
	virtual ~IStatementAST() = default;
	virtual void Accept(IStatementNodeVisitor& visitor) const = 0;
};

class VariableDeclarationAST : public IStatementAST
{
public:
	VariableDeclarationAST(std::unique_ptr<IdentifierExpressionAST> && identifier, const ExpressionType& type)
		: mIdentifier(std::move(identifier))
		, mType(type)
	{
	}

	void SetExpression(std::unique_ptr<IExpressionAST> && expression)
	{
		mExpression = std::move(expression);
	}

	const IExpressionAST* GetExpression() const
	{
		return mExpression.get();
	}

	const IdentifierExpressionAST& GetIdentifier() const
	{
		return *mIdentifier;
	}

	const ExpressionType& GetType() const
	{
		return mType;
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IdentifierExpressionAST> mIdentifier;
	std::unique_ptr<IExpressionAST> mExpression; // can be nullptr
	ExpressionType mType;
};

class AssignStatementAST : public IStatementAST
{
public:
	AssignStatementAST(
		std::unique_ptr<IdentifierExpressionAST> && identifier,
		std::unique_ptr<IExpressionAST> && expression
	)
		: mIdentifier(std::move(identifier))
		, mExpression(std::move(expression))
	{
	}

	const IdentifierExpressionAST& GetIdentifier() const
	{
		return *mIdentifier;
	}

	const IExpressionAST& GetExpression() const
	{
		return *mExpression;
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IdentifierExpressionAST> mIdentifier;
	std::unique_ptr<IExpressionAST> mExpression;
};

class IfStatementAST : public IStatementAST
{
public:
	IfStatementAST(
		std::unique_ptr<IExpressionAST> && expression,
		std::unique_ptr<IStatementAST> && then,
		std::unique_ptr<IStatementAST> && elif = nullptr
	)
		: mExpression(std::move(expression))
		, mThen(std::move(then))
		, mElse(std::move(elif))
	{
	}

	const IExpressionAST& GetExpression() const
	{
		return *mExpression;
	}

	const IStatementAST& GetThenStatement() const
	{
		return *mThen;
	}

	const IStatementAST* GetElseStatement() const
	{
		return mElse.get();
	}

	void SetElseClause(std::unique_ptr<IStatementAST> && elseClause)
	{
		mElse = std::move(elseClause);
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mExpression;
	std::unique_ptr<IStatementAST> mThen;
	std::unique_ptr<IStatementAST> mElse; // can be nullptr
};

class WhileStatementAST : public IStatementAST
{
public:
	WhileStatementAST(std::unique_ptr<IExpressionAST> && expression, std::unique_ptr<IStatementAST> && statement)
		: mExpression(std::move(expression))
		, mStatement(std::move(statement))
	{
	}

	const IExpressionAST& GetExpression() const
	{
		return *mExpression;
	}

	const IStatementAST& GetStatement() const
	{
		return *mStatement;
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mExpression;
	std::unique_ptr<IStatementAST> mStatement;
};

class PrintStatementAST : public IStatementAST
{
public:
	explicit PrintStatementAST(std::unique_ptr<IExpressionAST> && expression)
		: mExpression(std::move(expression))
	{
	}

	const IExpressionAST& GetExpression() const
	{
		return *mExpression;
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::unique_ptr<IExpressionAST> mExpression;
};

class CompositeStatementAST : public IStatementAST
{
public:
	std::size_t GetStatementsCount() const
	{
		return mStatements.size();
	}

	void AddStatement(std::unique_ptr<IStatementAST> && statement)
	{
		mStatements.push_back(std::move(statement));
	}

	const IStatementAST& GetStatement(std::size_t index) const
	{
		if (index > mStatements.size())
		{
			throw std::runtime_error("index must be less than statements count");
		}
		return *mStatements[index];
	}

	void Accept(IStatementNodeVisitor& visitor) const override
	{
		visitor.Visit(*this);
	}

private:
	std::vector<std::unique_ptr<IStatementAST>> mStatements;
};
