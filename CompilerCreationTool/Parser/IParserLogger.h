#pragma once
#include <string>

class IParserLogger
{
public:
	enum MessageCategory
	{
		Action = 1 << 0,
		Regular = 1 << 1,
		All = Action | Regular
	};

public:
	virtual ~IParserLogger() = default;
	virtual void SetMask(int mask) = 0;
	virtual int GetMask() const = 0;
	virtual void Log(const std::string& message, MessageCategory category = All) = 0;
	virtual void Clear() = 0;
};
