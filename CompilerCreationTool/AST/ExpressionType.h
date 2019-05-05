#pragma once
#include <string>

struct ExpressionType
{
	enum Value
	{
		Int,
		Float,
		Bool,
		String
	};

	ExpressionType(Value value)
		: value(value)
	{
	}

	Value value;
};

bool operator ==(const ExpressionType& left, const ExpressionType& right);
bool operator !=(const ExpressionType& left, const ExpressionType& right);
std::string ToString(const ExpressionType& type);
