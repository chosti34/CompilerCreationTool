#include "pch.h"
#include "Language.h"
#include "../Parser/Parser.h"
#include "../Parser/ParserTable.h"
#include "../Lexer/Lexer.h"
#include "../Grammar/GrammarUtils.h"
#include "LanguageInformation.h"

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

bool Language::IsInitialized() const
{
	return m_grammar && m_lexer && m_parser && m_information;
}

void Language::SetGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar)
{
	if (grammar != nullptr)
	{
		// Замеряем время выполнения инициализации
		auto beginTime = std::chrono::steady_clock::now();

		// Выполняем инициализацию пользовательского языка по грамматике
		m_grammar = std::move(grammar);
		m_lexer = CreateDefaultLexer(*m_grammar);
		m_parser = std::make_unique<Parser>(ParserTable::Create(*m_grammar), *m_lexer);
		m_parser->SetActionNames(GatherAllActions(*m_grammar));

		auto endTime = std::chrono::steady_clock::now();
		auto elapsedSeconds = std::chrono::duration<double>(endTime - beginTime);

		m_information = std::make_unique<LanguageInformation>(*m_lexer, *m_parser, *m_grammar, elapsedSeconds);
	}
	else
	{
		m_grammar = nullptr;
		m_lexer = nullptr;
		m_parser = nullptr;
		m_information = nullptr;
	}
}

const grammarlib::IGrammar& Language::GetGrammar() const
{
	assert(m_grammar);
	return *m_grammar;
}

IParser<bool>& Language::GetParser()
{
	assert(m_parser);
	return *m_parser;
}

const IParser<bool>& Language::GetParser() const
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
	assert(m_information);
	return *m_information;
}
