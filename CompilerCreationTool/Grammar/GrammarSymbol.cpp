#include "stdafx.h"
#include "GrammarSymbol.h"

using namespace grammarlib;

GrammarSymbol::GrammarSymbol(
	const std::string& name,
	GrammarSymbolType type,
	boost::optional<std::string> attribute
)
	: m_name(name)
	, m_type(type)
{
}

const std::string& GrammarSymbol::GetName() const
{
	return m_name;
}

GrammarSymbolType GrammarSymbol::GetType() const
{
	return m_type;
}

void grammarlib::GrammarSymbol::SetAttribute(boost::optional<std::string> attribute)
{
	m_attribute = std::move(attribute);
}

boost::optional<std::string> GrammarSymbol::GetAttribute() const
{
	return m_attribute;
}
