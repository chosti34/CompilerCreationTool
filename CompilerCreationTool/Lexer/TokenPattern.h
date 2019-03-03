#pragma once
#include <string>
#include <regex>

class TokenPattern
{
public:
	TokenPattern(const std::string& name, const std::string& origin, bool isEnding = false);

	void SetName(const std::string& name);
	const std::string& GetName() const;

	bool SetOrigin(const std::string& origin);
	const std::string& GetOrigin() const;

	const std::regex& GetRegex() const;
	bool IsEnding() const;

private:
	std::string m_name;
	std::string m_origin;
	std::regex m_regex;
	bool m_isEnding;
};

const std::vector<TokenPattern> &GetPredefinedPatterns();
