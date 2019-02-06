#include "stdafx.h"
#include "Entity.h"

using namespace grammarlib;

Entity::Entity(const std::string& name, Type type,
	boost::optional<std::string> attribute)
	: m_name(name)
	, m_type(type)
{
}

const std::string& Entity::GetName() const
{
	return m_name;
}

Entity::Type Entity::GetType() const
{
	return m_type;
}

boost::optional<std::string> Entity::GetAttribute() const
{
	return m_attribute;
}
