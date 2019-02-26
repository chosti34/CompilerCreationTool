#pragma once
#include <string>
#include <regex>

class TokenPattern
{
public:
	TokenPattern(
		const std::string& name,
		const std::string& origin,
		bool isEnding = false,
		int predefinedIndex = -1
	);

	const std::string& GetName() const;
	const std::string& GetOrigin() const;
	const std::regex& GetRegex() const;
	bool IsEnding() const;
	int GetPredefinedIndex() const;

private:
	int m_predefinedIndex;
	bool m_isEnding;
	std::string m_name;
	std::string m_origin;
	std::regex m_regex;
};

std::vector<TokenPattern> const& GetPredefinedPatterns();
