#pragma once
#include "IGrammarSymbol.h"

namespace grammarlib
{
class GrammarSymbol : public IGrammarSymbol
{
public:
	GrammarSymbol(
		const std::string& name,
		GrammarSymbolType type,
		boost::optional<std::string> attribute = boost::none
	);

	const std::string& GetName() const override;
	GrammarSymbolType GetType() const override;

	void SetAttribute(boost::optional<std::string> attribute) override;
	boost::optional<std::string> GetAttribute() const override;

private:
	GrammarSymbolType m_type;
	std::string m_name;
	boost::optional<std::string> m_attribute;
};
}
