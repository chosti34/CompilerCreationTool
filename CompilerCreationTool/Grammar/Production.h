#pragma once
#include "Entity.h"
#include <vector>

namespace grammarlib
{
class Production
{
public:
	Production(const std::string& left, const std::vector<Entity> & right);

	const std::string& GetLeftPart() const;
	const std::vector<Entity> & GetRightPart() const;

	size_t GetEntitiesCount() const;
	const Entity& GetEntity(size_t index) const;
	bool EndsWith(Entity::Type type) const;

	const Entity& GetFrontEntity() const;
	const Entity& GetBackEntity() const;

private:
	std::string m_left;
	std::vector<Entity> m_right;
};
}
