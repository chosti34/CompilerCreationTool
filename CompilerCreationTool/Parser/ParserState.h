#pragma once
#include "IParserState.h"

class ParserState : public IParserState
{
public:
	bool GetShiftFlag() const override;
	void SetShiftFlag(bool shift) override;

	bool GetPushFlag() const override;
	void SetPushFlag(bool push) override;

	bool GetErrorFlag() const override;
	void SetErrorFlag(bool error) override;

	bool GetEndFlag() const override;
	void SetEndFlag(bool end) override;

	const std::string& GetName() const override;
	void SetName(const std::string& name) override;

	boost::optional<size_t> GetNextAddress() const override;
	void SetNextAddress(boost::optional<size_t> address) override;

	const std::set<std::string>& GetAcceptableTerminals() const override;
	void SetAcceptableTerminals(const std::set<std::string>& terminals) override;
	bool AcceptsTerminal(const std::string & terminal) const override;

private:
	bool m_shift;
	bool m_push;
	bool m_error;
	bool m_end;
	std::string m_name;
	boost::optional<size_t> m_nextAddress;
	std::set<std::string> m_acceptables;
};
