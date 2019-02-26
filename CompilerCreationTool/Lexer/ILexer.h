#pragma once
#include "Token.h"
#include "TokenPattern.h"

class ILexer
{
public:
	virtual ~ILexer() = default;

	virtual Token GetNextToken() = 0;
	virtual void SetText(const std::string& text) = 0;
	
	virtual void AppendPattern(const TokenPattern& pattern) = 0;
	virtual void SetPattern(size_t index, const TokenPattern& newValue) = 0;
	virtual void SwapPatterns(size_t lhs, size_t rhs) = 0;

	virtual const TokenPattern& GetPattern(size_t index) const = 0;
	virtual size_t GetPatternsCount() const = 0;
};
