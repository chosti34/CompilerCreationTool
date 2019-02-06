#pragma once
#include <string>
#include <boost/optional.hpp>

namespace grammarlib
{
class Entity
{
public:
	enum Type
	{
		Epsilon,
		Terminal,
		Nonterminal
	};

public:
	Entity(const std::string& name, Type type,
		boost::optional<std::string> attribute = boost::none);

	const std::string& GetName() const;
	Type GetType() const;
	boost::optional<std::string> GetAttribute() const;

private:
	Type m_type;
	std::string m_name;
	boost::optional<std::string> m_attribute;
};
}
