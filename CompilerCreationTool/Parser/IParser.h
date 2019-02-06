#pragma once
#include <string>

template <typename T>
class IParser
{
public:
	virtual ~IParser() = default;
	virtual T Parse(const std::string& text) = 0;
};
