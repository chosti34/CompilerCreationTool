#pragma once
#include "IParser.h"

class Parser : public IParser<bool>
{
public:
	bool Parse(const std::string& text) override;
};
