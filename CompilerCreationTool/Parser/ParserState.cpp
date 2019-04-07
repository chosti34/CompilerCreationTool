#include "stdafx.h"
#include "ParserState.h"

ParserState::ParserState()
{
	m_flags = {
		{ StateFlag::End, false },
		{ StateFlag::Push, false },
		{ StateFlag::Error, false },
		{ StateFlag::Shift, false },
		{ StateFlag::Attribute, false }
	};
}

ParserState::~ParserState()
{
}

bool ParserState::GetFlag(StateFlag flag) const
{
	auto found = m_flags.find(flag);
	assert(found != m_flags.end());
	return found->second;
}

void ParserState::SetFlag(StateFlag flag, bool value)
{
	auto found = m_flags.find(flag);
	assert(found != m_flags.end());
	found->second = value;
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
