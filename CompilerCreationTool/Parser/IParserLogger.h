#pragma once
#include <string>

class IParserLogger
{
public:
	virtual ~IParserLogger() = default;
	virtual void Log(const std::string& message) = 0;
	virtual void Clear() = 0;
};
