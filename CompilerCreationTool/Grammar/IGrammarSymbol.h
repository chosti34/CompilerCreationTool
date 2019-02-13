#pragma once
#include "GrammarSymbolType.h"
#include <boost/optional.hpp>
#include <string>

namespace grammarlib
{
class IGrammarSymbol
{
public:
	virtual ~IGrammarSymbol() = default;

	virtual const std::string& GetName() const = 0;
	virtual GrammarSymbolType GetType() const = 0;

	virtual void SetAttribute(boost::optional<std::string> attribute) = 0;
	virtual boost::optional<std::string> GetAttribute() const = 0;
};
}
