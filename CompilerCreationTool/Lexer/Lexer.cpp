#include "stdafx.h"
#include "Lexer.h"

#include <algorithm>
#include <cassert>
#include <cctype>

namespace
{
bool SearchAndSaveMatch(
	const std::regex& regex,
	Lexer::RegexMatchResults& match,
	const std::string& text,
	size_t fromPosition
)
{
	std::regex_constants::match_flag_type flags =
		std::regex_constants::match_flag_type::match_continuous;
	return std::regex_search(text.c_str() + fromPosition, match, regex, flags);
}
}

Token Lexer::GetNextToken()
{
	while (mPosition < mText.length())
	{
		if (std::isspace(mText[mPosition]))
		{
			SkipWhitespaces();
			continue;
		}

		for (size_t i = 0; i < mPatterns.size(); ++i)
		{
			const TokenPattern& pattern = mPatterns[i];
			if (pattern.IsEnding())
			{
				continue;
			}

			if (SearchAndSaveMatch(pattern.GetRegex(), mMatchResults, mText, mPosition))
			{
				const std::string value = mMatchResults.str();
				Token token = { pattern.GetName(), value };
				mPosition += value.length();
				return token;
			}
		}

		throw std::invalid_argument("lexer can't parse token");
	}

	for (const TokenPattern& pattern : mPatterns)
	{
		if (pattern.IsEnding())
		{
			return { pattern.GetName() };
		}
	}
	throw std::logic_error("LOGIC ERROR: lexer doesn't have ending token to return");
}

void Lexer::SetText(const std::string& text)
{
	mText = text;
	mPosition = 0;
}

void Lexer::AppendPattern(const TokenPattern& pattern)
{
	mPatterns.push_back(pattern);
}

void Lexer::SwapPatterns(size_t lhs, size_t rhs)
{
	std::iter_swap(mPatterns.begin() + lhs, mPatterns.begin() + rhs);
}

void Lexer::SetPattern(size_t index, const TokenPattern& newValue)
{
	if (index >= mPatterns.size())
	{
		throw std::out_of_range("index must be less than patterns count");
	}
	mPatterns[index] = newValue;
}

const TokenPattern& Lexer::GetPattern(size_t index) const
{
	if (index >= mPatterns.size())
	{
		throw std::out_of_range("index must be less than patterns count");
	}
	return mPatterns[index];
}

TokenPattern& Lexer::GetPattern(size_t index)
{
	if (index >= mPatterns.size())
	{
		throw std::out_of_range("index must be less than patterns count");
	}
	return mPatterns[index];
}

size_t Lexer::GetPatternsCount() const
{
	return mPatterns.size();
}

void Lexer::SkipWhitespaces()
{
	while (mPosition < mText.length() && std::isspace(mText[mPosition]))
	{
		++mPosition;
	}
}
