#pragma once
#include <string>

class ICodegenLogger
{
public:
	virtual ~ICodegenLogger() = default;
	virtual void Log(const std::string& message) = 0;
};
