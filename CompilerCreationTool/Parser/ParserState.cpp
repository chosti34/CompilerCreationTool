#include "stdafx.h"
#include "ParserState.h"

bool ParserState::GetShiftFlag() const
{
	return m_shift;
}

void ParserState::SetShiftFlag(bool shift)
{
	m_shift = shift;
}

bool ParserState::GetPushFlag() const
{
	return m_push;
}

void ParserState::SetPushFlag(bool push)
{
	m_push = push;
}

bool ParserState::GetErrorFlag() const
{
	return m_error;
}

void ParserState::SetErrorFlag(bool error)
{
	m_error = error;
}

bool ParserState::GetEndFlag() const
{
	return m_end;
}

void ParserState::SetEndFlag(bool end)
{
	m_end = end;
}

const std::string& ParserState::GetName() const
{
	return m_name;
}

void ParserState::SetName(const std::string& name)
{
	m_name = name;
}

boost::optional<size_t> ParserState::GetNextAddress() const
{
	return m_nextAddress;
}

void ParserState::SetNextAddress(boost::optional<size_t> address)
{
	m_nextAddress = address;
}

const std::set<std::string>& ParserState::GetAcceptableTerminals() const
{
	return m_acceptables;
}

void ParserState::SetAcceptableTerminals(const std::set<std::string>& terminals)
{
	m_acceptables = terminals;
}

bool ParserState::AcceptsTerminal(const std::string& terminal) const
{
	return m_acceptables.find(terminal) != m_acceptables.end();
}
