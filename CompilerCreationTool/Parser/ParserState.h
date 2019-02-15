#pragma once
#include "IParserState.h"
#include <unordered_map>

class ParserState : public IParserState
{
public:
	ParserState();
	~ParserState();

	bool GetFlag(StateFlag flag) const override;
	void SetFlag(StateFlag flag, bool value) override;

	const std::string& GetName() const override;
	void SetName(const std::string& name) override;

	boost::optional<size_t> GetNextAddress() const override;
	void SetNextAddress(boost::optional<size_t> address) override;

	const std::set<std::string>* GetAcceptableTerminals() const override;
	void SetAcceptableTerminals(const std::set<std::string>& terminals) override;
	bool AcceptsTerminal(const std::string & terminal) const override;

private:
	std::string m_name;
	boost::optional<size_t> m_nextAddress;
	std::unordered_map<StateFlag, bool> m_flags;
	boost::optional<std::set<std::string>> m_acceptables;
};
