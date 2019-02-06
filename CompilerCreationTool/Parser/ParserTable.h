#pragma once
#include <vector>
#include <memory>

class ParserState;
class Grammar;

class ParserTable
{
public:
	ParserTable() = delete;
	ParserTable& operator =(const ParserTable& table) = delete;

	void AddState(std::unique_ptr<ParserState> && state);

	ParserState& GetState(size_t index);
	const ParserState& GetState(size_t index) const;
	size_t GetStatesCount() const;

	static std::unique_ptr<ParserTable> Create(const Grammar& grammar);

private:
	std::vector<std::unique_ptr<ParserState>> m_states;
};
