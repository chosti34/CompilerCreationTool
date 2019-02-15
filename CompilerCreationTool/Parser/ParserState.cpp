#include "stdafx.h"
#include "ParserState.h"

int ParserState::GetFlags() const
{
	return m_flags;
}

void ParserState::SetFlags(int flags)
{
	m_flags = flags;
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

const std::set<std::string>* ParserState::GetAcceptableTerminals() const
{
	if (bool(m_acceptables))
	{
		return m_acceptables.get_ptr();
	}
	return nullptr;
}

void ParserState::SetAcceptableTerminals(const std::set<std::string>& terminals)
{
	m_acceptables = terminals;
}

bool ParserState::AcceptsTerminal(const std::string& terminal) const
{
	if (!bool(m_acceptables))
	{
		throw std::runtime_error("acceptable terminals aren't defined yet");
	}
	return m_acceptables->find(terminal) != m_acceptables->end();
}
