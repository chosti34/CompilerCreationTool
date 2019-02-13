#pragma once
#include "IGrammar.h"

#include <set>
#include <functional>

namespace grammarlib
{
// �������� ������ ������ ��������� � ���������� � ������ ������������ � ����� �����
size_t GetProductionIndex(const IGrammar& grammar, const std::string& nonterminal);
// ����� �� ������� � ������ �������� ������������ ���� �� ������ ������ ����������
bool ProductionHasAlternativeWithHigherIndex(const IGrammar& grammar, size_t index);

// �������� ��� ������� ������ ���������� � �������� ����� ������
std::set<int> GatherProductionIndices(const IGrammar& grammar, const std::string& nonterminal);
// �������� ��� ������� ������ ����������, ��������������� ��������� ���������
std::set<int> GatherProductionIndices(const IGrammar& grammar, std::function<bool(const IGrammarProduction&)> && predicate);

// ������� �� ������ ����� ������� ������ �� ������������
bool ProductionConsistsOfNonterminals(const IGrammarProduction& production);
// ���������� �� ������ ������� � �������� ����� ������
bool ExistsEpsilonProduction(const IGrammar& grammar, const std::string& nonterminal);
// ����� �� ���� ���������� ������ (�������� ��������������)
bool NonterminalHasEmptiness(const IGrammar& grammar, const std::string& nonterminal);

// �������� �������������� ��������� ����������� � ����������
std::set<std::pair<int, int>> GatherNonterminalOccurrences(const IGrammar& grammar, const std::string& nonterminal);

// �������� ������������ ��������� �����������
std::set<std::string> GatherBeginningSymbolsOfNonterminal(const IGrammar& grammar, const std::string& nonterminal);
// �������� ������������ ��������� ��� ���������� �������
std::set<std::string> GatherBeginningSymbolsOfProduction(const IGrammar& grammar, int productionIndex);
// �������� �������-����������� �����������
std::set<std::string> GatherFollowingSymbols(const IGrammar& grammar, const std::string& nonterminal);
}
