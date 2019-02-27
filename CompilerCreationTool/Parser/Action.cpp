#include "stdafx.h"
#include "Action.h"

Action::Action(const std::string& name, ActionType type)
	: m_type(type)
	, m_name(name)
{
}

const std::string& Action::GetName() const
{
	return m_name;
}

ActionType Action::GetType() const
{
	return m_type;
}
