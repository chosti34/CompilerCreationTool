#pragma once
#include <string>
#include <regex>

class TokenPattern
{
public:
	TokenPattern(
		const std::string& name,
		const std::string& origin,
		bool isEnding = false
	);

	const std::string& GetName() const;
	const std::string& GetOrigin() const;
	const std::regex& GetRegex() const;
	bool IsEnding() const;

private:
	bool mIsEnding;
	std::string mName;
	std::string mOrigin;
	std::regex mRegex;
};
