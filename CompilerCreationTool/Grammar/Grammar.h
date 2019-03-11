#pragma once
#include "IGrammar.h"
#include <vector>

namespace grammarlib
{
class Grammar : public IGrammar
{
public:
	bool IsNormalized() const override;
	void NormalizeIndices() override;

	void AddProduction(std::unique_ptr<IGrammarProduction> && production) override;
	const IGrammarProduction& GetProduction(size_t index) const override;

	size_t GetProductionsCount() const override;
	bool IsEmpty() const override;

	const std::string& GetStartSymbol() const override;
	const std::string& GetEndTerminal() const override;

private:
	void TryInsertFirstProduction(std::unique_ptr<IGrammarProduction> && production);
	void TryInsertProduction(std::unique_ptr<IGrammarProduction> && production);

private:
	std::vector<std::unique_ptr<IGrammarProduction>> mProductions;
	bool mNormalized = false;
};
}
