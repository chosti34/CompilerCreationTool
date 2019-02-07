#pragma once
#include "Fwd.h"

#include <set>
#include <string>
#include <functional>

namespace grammarlib
{
// �������� ��� ������� ������ ���������� � �������� ����� ������
std::set<int> GatherProductionIndices(const Grammar& grammar, const std::string& nonterminal);
// �������� ��� ������� ������ ����������, ��������������� ��������� ���������
std::set<int> GatherProductionIndices(const Grammar& grammar, std::function<bool(const Production&)> && predicate);

// ������� �� ������ ����� ������� ������ �� ������������
bool ProductionConsistsOfNonterminals(const Production& production);
// ���������� �� ������ ������� � �������� ����� ������
bool ExistsEpsilonProduction(const Grammar& grammar, const std::string& nonterminal);
// ����� �� ���� ���������� ������ (�������� ��������������)
bool NonterminalHasEmptiness(const Grammar& grammar, const std::string& nonterminal);

// �������� �������������� ��������� ����������� � ����������
std::set<std::pair<int, int>> GatherNonterminalOccurrences(const Grammar& grammar, const std::string& nonterminal);

// �������� ������������ ��������� �����������
std::set<std::string> GatherBeginningSymbolsOfNonterminal(const Grammar& grammar, const std::string& nonterminal);
// �������� ������������ ��������� ��� ���������� �������
std::set<std::string> GatherBeginningSymbolsOfProduction(const Grammar& grammar, int productionIndex);
// �������� �������-����������� �����������
std::set<std::string> GatherFollowingSymbols(const Grammar& grammar, const std::string& nonterminal);
}
