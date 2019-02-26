#pragma once
#include "ILexer.h"
#include <vector>

class Lexer : public ILexer
{
public:
	using RegexMatchResults = std::match_results<const char*>;

public:
	Token GetNextToken() override;
	void SetText(const std::string& text) override;

	void AppendPattern(const TokenPattern& pattern) override;
	void SetPattern(size_t index, const TokenPattern& newValue) override;
	void SwapPatterns(size_t lhs, size_t rhs) override;

	const TokenPattern& GetPattern(size_t index) const override;
	size_t GetPatternsCount() const override;

private:
	void SkipWhitespaces();

private:
	size_t mPosition;
	std::string mText;
	RegexMatchResults mMatchResults;
	std::vector<TokenPattern> mPatterns;
};
