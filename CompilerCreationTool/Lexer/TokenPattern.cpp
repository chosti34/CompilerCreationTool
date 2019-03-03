#include "stdafx.h"
#include "TokenPattern.h"
#include <unordered_map>
#include <cassert>
#include <vector>

namespace
{
const std::vector<TokenPattern> gcPredefinedPatterns = {
	{ "Identifier", "id" },
	{ "Integer", "[1-9][0-9]*" },
	{ "Float", "[1-9]\\.[0-9]+" }
};

bool RegexAllowsEmptyString(const std::regex& regex)
{
	return std::regex_match("", regex);
}
}

TokenPattern::TokenPattern(
	const std::string& name,
	const std::string& origin,
	bool isEnding
)
	: m_name(name)
	, m_origin(origin)
	, m_regex(origin)
	, m_isEnding(isEnding)
{
	// Пользователь не может создать TokenPattern в runtime, поэтому используется assert
	assert(!RegexAllowsEmptyString(m_regex));
}

void TokenPattern::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& TokenPattern::GetName() const
{
	return m_name;
}

bool TokenPattern::SetOrigin(const std::string& origin)
{
	std::regex newRegex(origin);
	if (!RegexAllowsEmptyString(newRegex))
	{
		m_regex = std::move(newRegex);
		m_origin = origin;
		return true;
	}
	return false;
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

const std::vector<TokenPattern> &GetPredefinedPatterns()
{
	return gcPredefinedPatterns;
}
