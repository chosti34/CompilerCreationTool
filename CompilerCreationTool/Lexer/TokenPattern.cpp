#include "stdafx.h"
#include "TokenPattern.h"

TokenPattern::TokenPattern(
	const std::string& name,
	const std::string& origin,
	bool isEnding
)
	: mName(name)
	, mOrigin(origin)
	, mIsEnding(isEnding)
	, mRegex(origin)
{
}

const std::string& TokenPattern::GetName() const
{
	return mName;
}

const std::string& TokenPattern::GetOrigin() const
{
	return mOrigin;
}

const std::regex& TokenPattern::GetRegex() const
{
	return mRegex;
}

bool TokenPattern::IsEnding() const
{
	return mIsEnding;
}
