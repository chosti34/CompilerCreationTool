#pragma once
#include "IGrammarProduction.h"
#include <vector>

namespace grammarlib
{
class GrammarProduction : public IGrammarProduction
{
public:
	GrammarProduction(
		const std::string& left,
		std::vector<std::unique_ptr<IGrammarSymbol>> && right
	);

	const std::string& GetLeftPart() const override;
	bool EndsWith(GrammarSymbolType type) const override;

	size_t GetSymbolsCount() const override;
	const IGrammarSymbol& GetSymbol(size_t index) const override;

	const IGrammarSymbol& GetFrontSymbol() const override;
	const IGrammarSymbol& GetBackSymbol() const override;

private:
	std::string m_left;
	std::vector<std::unique_ptr<IGrammarSymbol>> m_right;
};
}
