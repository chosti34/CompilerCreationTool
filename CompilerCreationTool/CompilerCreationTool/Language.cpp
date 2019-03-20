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

	/*auto lexer = CreateDefaultLexer(*grammar);
	auto parser = std::make_unique<Parser>(ParserTable::Create(*grammar), *lexer);
	parser->SetActionNames(GatherAllActions(*grammar));

	if (IsInitialized())
	{
		for (size_t i = 0; i < lexer->GetPatternsCount(); ++i)
		{
			const TokenPattern& oldPattern = lexer->GetPattern(i);
			if (auto pos = m_lexer->GetPatternPos(oldPattern.GetName()))
			{
				lexer->SetPattern(m_lexer->GetPattern(*pos));
			}
		}

		for (size_t i = 0; i < parser->GetActionsCount(); ++i)
		{
			const IAction& oldAction = parser->GetAction(i);
			if (auto pos = m_parser->GetActionPos(oldAction.GetName()))
			{
				IAction& newAction = parser->GetAction(*pos);
				newAction.SetName(oldAction.GetName());
				newAction.SetType(oldAction.GetType());
			}
		}
	}*/

	// Замеряем время выполнения инициализации
	auto beginTime = std::chrono::steady_clock::now();

	// Выполняем инициализацию пользовательского языка по грамматике
	m_grammar = std::move(grammar);
	m_lexer = CreateDefaultLexer(*m_grammar);
	m_parser = std::make_unique<Parser>(ParserTable::Create(*m_grammar), *m_lexer);
	m_parser->SetActionNames(GatherAllActions(*m_grammar));

	auto endTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>(endTime - beginTime);

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
