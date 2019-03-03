#include "stdafx.h"
#include "Action.h"

Action::Action(const std::string& name, ActionType type)
	: m_type(type)
	, m_name(name)
{
}

void Action::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& Action::GetName() const
{
	return m_name;
}

void Action::SetType(ActionType type)
{
	m_type = type;
}

ActionType Action::GetType() const
{
	return m_type;
}
