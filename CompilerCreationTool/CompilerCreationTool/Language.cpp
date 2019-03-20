#include "pch.h"
#include "Language.h"
#include "LanguageInformation.h"

#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include "../Parser/ParserTable.h"
#include "../Grammar/GrammarUtils.h"

using namespace grammarlib;

namespace
{
// Инициализируем лексер и заполняем "шаблоны для разбора токенов" по умолчанию:
//  каждое регулярное выражение токена есть название терминала
std::unique_ptr<ILexer> CreateDefaultLexer(const IGrammar& grammar)
{
	auto isTerminal = [](const IGrammarSymbol& symbol) {
		return symbol.GetType() == GrammarSymbolType::Terminal;
	};

	auto lexer = std::make_unique<Lexer>();
	for (const std::string& terminal : GatherSymbols(grammar, isTerminal))
	{
		TokenPattern pattern(terminal, terminal, terminal == grammar.GetEndTerminal());
		lexer->AppendPattern(pattern);
	}
	return lexer;
}

template <typename T>
bool Contains(const std::set<T> &set, const T& value)
{
	return set.find(value) != set.end();
}
}

Language::Language()
{
	Reset();
}

void Language::Reset()
{
	m_grammar = nullptr;
	m_lexer = nullptr;
	m_parser = nullptr;
	m_info = nullptr;
}

bool Language::IsInitialized() const
{
	return m_grammar && m_lexer && m_parser && m_info;
}

void Language::SetGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar)
{
	assert(grammar);

	// Замеряем время выполнения инициализации
	auto beginTime = std::chrono::steady_clock::now();

	// Выполняем инициализацию пользовательского языка по грамматике
	auto lexer = CreateDefaultLexer(*grammar);
	auto parser = std::make_unique<Parser>(ParserTable::Create(*grammar), *lexer);
	parser->SetActionNames(GatherAllActions(*grammar));

	auto endTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>(endTime - beginTime);

	if (IsInitialized())
	{
		std::vector<std::pair<int, int>> indices;
		std::set<int> swapped;

		for (std::size_t i = 0; i < lexer->GetPatternsCount(); ++i)
		{
			const TokenPattern& newPattern = lexer->GetPattern(i);
			if (auto pos = m_lexer->GetPatternPos(newPattern.GetName()))
			{
				lexer->SetPattern(i, m_lexer->GetPattern(*pos));
				indices.emplace_back(i, std::min(*pos, lexer->GetPatternsCount() - 1));
			}
		}

		for (std::size_t i = 0; i < indices.size(); ++i)
		{
			if (!Contains(swapped, indices[i].first) && !Contains(swapped, indices[i].second))
			{
				lexer->SwapPatterns(indices[i].first, indices[i].second);
				swapped.insert(indices[i].first);
				swapped.insert(indices[i].second);
			}
		}

		indices.clear();
		swapped.clear();

		for (size_t i = 0; i < parser->GetActionsCount(); ++i)
		{
			IAction& newAction = parser->GetAction(i);
			if (auto pos = m_parser->GetActionPos(newAction.GetName()))
			{
				IAction& oldAction = m_parser->GetAction(*pos);
				newAction.SetName(oldAction.GetName());
				newAction.SetType(oldAction.GetType());
				indices.emplace_back(i, std::min(*pos, parser->GetActionsCount() - 1));
			}
		}

		for (std::size_t i = 0; i < indices.size(); ++i)
		{
			if (!Contains(swapped, indices[i].first) && !Contains(swapped, indices[i].second))
			{
				parser->SwapActions(indices[i].first, indices[i].second);
				swapped.insert(indices[i].first);
				swapped.insert(indices[i].second);
			}
		}
	}

	m_grammar = std::move(grammar);
	m_lexer = std::move(lexer);
	m_parser = std::move(parser);
	m_info = std::make_unique<LanguageInformation>(*m_lexer, *m_parser, *m_grammar, elapsedSeconds);
}

const grammarlib::IGrammar& Language::GetGrammar() const
{
	assert(m_grammar);
	return *m_grammar;
}

IParser<ParseResults>& Language::GetParser()
{
	assert(m_parser);
	return *m_parser;
}

const IParser<ParseResults>& Language::GetParser() const
{
	assert(m_parser);
	return *m_parser;
}

ILexer& Language::GetLexer()
{
	assert(m_lexer);
	return *m_lexer;
}

const ILexer& Language::GetLexer() const
{
	assert(m_lexer);
	return *m_lexer;
}

const LanguageInformation& Language::GetInfo() const
{
	assert(m_info);
	return *m_info;
}
