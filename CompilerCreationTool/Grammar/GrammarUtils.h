#pragma once
#include "IGrammar.h"
#include <functional>
#include <vector>
#include <set>

namespace grammarlib
{
bool HasLeftRecursion(const IGrammar& grammar, const std::string& nonterminal);
unsigned CountProductions(const IGrammar& grammar, std::function<bool(const IGrammarProduction&)> && predicate);

// �������� ����� �������� ����������, ������� ������������� ���������, � ���� ������� � ���������������� ����������
std::vector<std::string> GatherSymbols(const IGrammar& grammar, std::function<bool(const IGrammarSymbol&)> && predicate);
// �������� ��� �������� (��� ��������) ���������� � ���� ������� � ���������������� ����������
std::vector<std::string> GatherAllActions(const IGrammar& grammar);

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
