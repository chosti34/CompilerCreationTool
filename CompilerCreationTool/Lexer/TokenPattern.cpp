#include "stdafx.h"
#include "TokenPattern.h"
#include <vector>

namespace
{
const std::vector<TokenPattern> gcPredefinedPatterns = {
	TokenPattern("Identifier", "id", false, 0),
	TokenPattern("Integer", "123", false, 1),
	TokenPattern("Float", "0\\.1", false, 2)
};
}

TokenPattern::TokenPattern(
	const std::string& name,
	const std::string& origin,
	bool isEnding,
	int predefinedIndex
)
	: m_name(name)
	, m_origin(origin)
	, m_isEnding(isEnding)
	, m_regex(origin)
	, m_predefinedIndex(predefinedIndex)
{
}

const std::string& TokenPattern::GetName() const
{
	return m_name;
}

const std::string& TokenPattern::GetOrigin() const
{
	return m_origin;
}

const std::regex& TokenPattern::GetRegex() const
{
	return m_regex;
}

bool TokenPattern::IsEnding() const
{
	return m_isEnding;
}

int TokenPattern::GetPredefinedIndex() const
{
	return m_predefinedIndex;
}

std::vector<TokenPattern> const& GetPredefinedPatterns()
{
	return gcPredefinedPatterns;
}
