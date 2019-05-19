#pragma once
#include "../AST/AST.h"
#include <boost/variant.hpp>

struct ParseResults
{
	bool success;
	std::unique_ptr<IExpressionAST> expression;
	std::unique_ptr<IStatementAST> statement;
	std::string error;
};
