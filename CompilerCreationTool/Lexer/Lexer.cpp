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

bool IsNewlineCharacter(char ch)
{
	return ch == '\n' || ch == '\r' || ch == '\r\n';
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

		for (const TokenPattern& pattern : mPatterns)
		{
			if (pattern.IsEnding())
			{
				continue;
			}

			if (SearchAndSaveMatch(pattern.GetRegex(), mMatchResults, mText, mPosition))
			{
				const std::string value = mMatchResults.str();
				Token token = { pattern.GetName(), value };
				UpdatePositions(value);
				return token;
			}
		}

		throw std::invalid_argument(
			"can't read token at line " +
			std::to_string(mLine + 1) + ", ch " +
			std::to_string(mColumn + 1)
		);
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
	mColumn = 0;
	mLine = 0;
}

void Lexer::AppendPattern(const TokenPattern& pattern)
{
#ifdef _DEBUG
	auto it = std::find_if(mPatterns.begin(), mPatterns.end(), [&pattern](const TokenPattern& p) {
		return p.GetName() == pattern.GetName();
	});
	assert(it == mPatterns.end());
#endif
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

boost::optional<size_t> Lexer::GetPatternPos(const std::string& name) const
{
	for (std::size_t index = 0; index < mPatterns.size(); ++index)
	{
		const TokenPattern& pattern = mPatterns[index];
		if (pattern.GetName() == name)
		{
			return boost::make_optional(index);
		}
	}
	return boost::none;
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
		++mColumn;
		if (IsNewlineCharacter(mText[mPosition]))
		{
			++mLine;
			mColumn = 0;
		}
		++mPosition;
	}
}

void Lexer::UpdatePositions(const std::string& str)
{
	for (char ch : str)
	{
		++mPosition;
		++mColumn;
		if (IsNewlineCharacter(ch))
		{
			++mLine;
			mColumn = 0;
		}
	}
}
