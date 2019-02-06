#pragma once
#include "Entity.h"
#include <vector>

namespace grammarlib
{
class Production
{
public:
	Production(const std::string& left, const std::vector<Entity> & right);

	const std::string& GetLeft() const;
	const std::vector<Entity> & GetRight() const;

	bool EndsWith(Entity::Type type) const;

private:
	std::string m_left;
	std::vector<Entity> m_right;
};
}
