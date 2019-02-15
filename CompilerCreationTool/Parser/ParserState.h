#pragma once
#include "IParserState.h"

class ParserState : public IParserState
{
public:
	int GetFlags() const override;
	void SetFlags(int flags) override;

	const std::string& GetName() const override;
	void SetName(const std::string& name) override;

	boost::optional<size_t> GetNextAddress() const override;
	void SetNextAddress(boost::optional<size_t> address) override;

	const std::set<std::string>* GetAcceptableTerminals() const override;
	void SetAcceptableTerminals(const std::set<std::string>& terminals) override;
	bool AcceptsTerminal(const std::string & terminal) const override;

private:
	int m_flags;
	std::string m_name;
	boost::optional<size_t> m_nextAddress;
	boost::optional<std::set<std::string>> m_acceptables;
};
