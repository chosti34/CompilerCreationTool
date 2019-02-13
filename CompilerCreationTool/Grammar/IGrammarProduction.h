#pragma once
#include "IGrammarSymbol.h"

namespace grammarlib
{
class IGrammarProduction
{
public:
	virtual ~IGrammarProduction() = default;

	virtual const std::string& GetLeftPart() const = 0;
	virtual bool EndsWith(GrammarSymbolType type) const = 0;

	virtual size_t GetSymbolsCount() const = 0;
	virtual const IGrammarSymbol& GetSymbol(size_t index) const = 0;

	virtual const IGrammarSymbol& GetFrontSymbol() const = 0;
	virtual const IGrammarSymbol& GetBackSymbol() const = 0;
};
}
