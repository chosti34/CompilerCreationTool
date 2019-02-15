#pragma once
#include "IParserTable.h"
#include "../Grammar/IGrammar.h"
#include <vector>

class ParserTable : public IParserTable
{
public:
	void AddState(std::unique_ptr<IParserState> && state) override;
	size_t GetStatesCount() const override;

	IParserState& GetState(size_t index) override;
	const IParserState& GetState(size_t index) const override;

	static std::unique_ptr<ParserTable> Create(const grammarlib::IGrammar& grammar);

private:
	std::vector<std::unique_ptr<IParserState>> m_states;
};
