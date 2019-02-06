#include "stdafx.h"
#include "ParserTable.h"
#include "ParserState.h"

void ParserTable::AddState(std::unique_ptr<ParserState> && state)
{
	m_states.push_back(std::move(state));
}

ParserState& ParserTable::GetState(size_t index)
{
	if (index >= m_states.size())
	{
		throw std::out_of_range("index must be less than states count");
	}
	return *m_states[index];
}

const ParserState& ParserTable::GetState(size_t index) const
{
	if (index >= m_states.size())
	{
		throw std::out_of_range("index must be less than states count");
	}
	return *m_states[index];
}

size_t ParserTable::GetStatesCount() const
{
	return m_states.size();
}

std::unique_ptr<ParserTable> ParserTable::Create(const Grammar& grammar)
{
	return nullptr;
}
