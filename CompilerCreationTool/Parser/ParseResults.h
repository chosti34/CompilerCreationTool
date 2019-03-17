#pragma once
#include "../AST/AST.h"

struct ParseResults
{
	bool success;
	std::unique_ptr<IExpressionAST> expression;
};
