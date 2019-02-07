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

const std::string& Production::GetLeftPart() const
{
	return m_left;
}

const std::vector<Entity> & Production::GetRightPart() const
{
	return m_right;
}

size_t Production::GetEntitiesCount() const
{
	return m_right.size();
}

const Entity& grammarlib::Production::GetEntity(size_t index) const
{
	if (index >= m_right.size())
	{
		throw std::out_of_range("index must be less than entities count");
	}
	return m_right[index];
}

bool Production::EndsWith(Entity::Type type) const
{
	assert(!m_right.empty());
	return m_right.back().GetType() == type;
}

const Entity& grammarlib::Production::GetFrontEntity() const
{
	assert(!m_right.empty());
	return m_right.front();
}

const Entity& grammarlib::Production::GetBackEntity() const
{
	assert(!m_right.empty());
	return m_right.back();
}
