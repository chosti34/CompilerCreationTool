#pragma once
#include "IGrammar.h"

#include <set>
#include <functional>

namespace grammarlib
{
// Получить индекс первой продукции в грамматике с данным нетерминалом в левой части
size_t GetProductionIndex(const IGrammar& grammar, const std::string& nonterminal);
// Имеет ли правило с данным индексом альтернативы ниже по списку правил грамматики
bool ProductionHasAlternativeWithHigherIndex(const IGrammar& grammar, size_t index);

// Получить все индексы правил грамматики с заданной левой частью
std::set<int> GatherProductionIndices(const IGrammar& grammar, const std::string& nonterminal);
// Получить все индексы правил грамматики, удовлетворяющих заданному предикату
std::set<int> GatherProductionIndices(const IGrammar& grammar, std::function<bool(const IGrammarProduction&)> && predicate);

// Состоит ли правая часть правила только из нетерминалов
bool ProductionConsistsOfNonterminals(const IGrammarProduction& production);
// Существует ли пустое правило с заданной левой частью
bool ExistsEpsilonProduction(const IGrammar& grammar, const std::string& nonterminal);
// Может ли быть нетерминал пустым (учитывая транзитивность)
bool NonterminalHasEmptiness(const IGrammar& grammar, const std::string& nonterminal);

// Получить грамматические вхождение нетерминала в грамматике
std::set<std::pair<int, int>> GatherNonterminalOccurrences(const IGrammar& grammar, const std::string& nonterminal);

// Получить направляющее множество нетерминала
std::set<std::string> GatherBeginningSymbolsOfNonterminal(const IGrammar& grammar, const std::string& nonterminal);
// Получить направляющее множество для указанного правила
std::set<std::string> GatherBeginningSymbolsOfProduction(const IGrammar& grammar, int productionIndex);
// Получить символы-следователи нетерминала
std::set<std::string> GatherFollowingSymbols(const IGrammar& grammar, const std::string& nonterminal);
}
