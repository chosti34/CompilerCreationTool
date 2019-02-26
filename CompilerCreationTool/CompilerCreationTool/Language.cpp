#include "pch.h"
#include "Language.h"
#include "../Parser/Parser.h"
#include "../Parser/ParserTable.h"
#include "../Lexer/Lexer.h"
#include "../Grammar/GrammarUtils.h"

namespace
{
// Инициализируем лексер и заполняем "шаблоны для разбора токенов" по умолчанию:
//  каждое регулярное выражение токена есть название терминала
std::unique_ptr<ILexer> CreateDefaultLexer(const grammarlib::IGrammar& grammar)
{
	auto lexer = std::make_unique<Lexer>();
	for (const std::string& terminal : grammarlib::GatherAllTerminals(grammar))
	{
		TokenPattern pattern(terminal, terminal, terminal == grammar.GetEndTerminal());
		lexer->AppendPattern(pattern);
	}
	return lexer;
}
}

bool Language::IsInitialized() const
{
	return m_grammar && m_lexer && m_parser;
}

void Language::SetGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar)
{
	m_grammar = std::move(grammar);
	m_lexer = CreateDefaultLexer(*m_grammar);
	m_parser = std::make_unique<Parser>(ParserTable::Create(*m_grammar), *m_lexer);
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
