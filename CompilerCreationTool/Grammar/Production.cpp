#include "stdafx.h"
#include "Production.h"

using namespace grammarlib;

Production::Production(const std::string& left, const std::vector<Entity> & right)
	: m_left(left)
	, m_right(right)
{
	if (right.empty())
	{
		throw std::runtime_error("grammar production's right part can't be empty");
	}
}

const std::string& Production::GetLeft() const
{
	return m_left;
}

const std::vector<Entity> & Production::GetRight() const
{
	return m_right;
}

bool Production::EndsWith(Entity::Type type) const
{
	assert(!m_right.empty());
	return m_right.back().GetType() == type;
}
