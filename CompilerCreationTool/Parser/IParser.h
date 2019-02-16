#pragma once
#include "IParserTable.h"
#include <string>

template <typename T = void>
class IParser
{
public:
	virtual ~IParser() = default;
	virtual T Parse(const std::string& text) = 0;
	virtual const IParserTable& GetTable() const = 0;
};
