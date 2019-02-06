#pragma once
#include <set>
#include <string>

class ParserState
{
public:
	static const int64_t NO_NEXT_ADDRESS;

public:
	void SetName(const std::string& name);
	const std::string& GetName() const;

	void SetShiftFlag(bool shift);
	bool GetShiftFlag() const;

	void SetHasAlternativesFlag(bool hasAlternatives);
	bool GetHasAlternativesFlag() const;

	void SetEndFlag(bool end);
	bool GetEndFlag() const;

	void SetNextAddress(int64_t next);
	int64_t GetNextAddress() const;

	const std::set<std::string> & GetAcceptableTerminals() const;
	bool AddAcceptableTerminal(const std::string& terminal);
	bool Accepts(const std::string& terminal);

private:
	std::string m_name;
	bool m_shift;
	bool m_hasAlternative;
	bool m_end;
	bool m_push;
	int64_t m_next;
	std::set<std::string> m_acceptableTerminals;
};
