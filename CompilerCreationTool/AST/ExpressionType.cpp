#include "stdafx.h"
#include "ExpressionType.h"
#include <cassert>

bool operator ==(const ExpressionType& left, const ExpressionType& right)
{
	return left.value == right.value;
}

bool operator !=(const ExpressionType& left, const ExpressionType& right)
{
	return !(left == right);
}

std::string ToString(const ExpressionType& type)
{
	switch (type.value)
	{
	case ExpressionType::Int:
		return "Int";
	case ExpressionType::Float:
		return "Float";
	case ExpressionType::Bool:
		return "Bool";
	case ExpressionType::String:
		return "String";
	default:
		assert(false);
		throw std::logic_error("can't convert undefined expression type to string");
	}
}
