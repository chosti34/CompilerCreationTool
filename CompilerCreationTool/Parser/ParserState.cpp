#include "stdafx.h"
#include "ParserState.h"

const int64_t ParserState::NO_NEXT_ADDRESS = -1;

void ParserState::SetName(const std::string& name)
{
	m_name = name;
}

const std::string & ParserState::GetName() const
{
	return m_name;
}

void ParserState::SetShiftFlag(bool shift)
{
	m_shift = shift;
}

bool ParserState::GetShiftFlag() const
{
	return m_shift;
}

void ParserState::SetHasAlternativesFlag(bool hasAlternatives)
{
	m_hasAlternative = hasAlternatives;
}

bool ParserState::GetHasAlternativesFlag() const
{
	return m_hasAlternative;
}

void ParserState::SetEndFlag(bool end)
{
	m_end = end;
}

bool ParserState::GetEndFlag() const
{
	return m_end;
}

void ParserState::SetNextAddress(int64_t next)
{
	m_next = next;
}

int64_t ParserState::GetNextAddress() const
{
	return m_next;
}

const std::set<std::string> & ParserState::GetAcceptableTerminals() const
{
	return m_acceptableTerminals;
}

bool ParserState::AddAcceptableTerminal(const std::string& terminal)
{
	auto inserted = m_acceptableTerminals.insert(terminal);
	return inserted.second;
}

bool ParserState::Accepts(const std::string& terminal)
{
	return m_acceptableTerminals.find(terminal) != m_acceptableTerminals.end();
}
