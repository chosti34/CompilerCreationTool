#include "pch.h"
#include "Language.h"
#include "../Parser/Parser.h"
#include "../Parser/ParserTable.h"
#include "../Lexer/Lexer.h"
#include "../Grammar/GrammarUtils.h"

namespace
{
// �������������� ������ � ��������� "������� ��� ������� �������" �� ���������:
//  ������ ���������� ��������� ������ ���� �������� ���������
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

void Language::SetLanguageGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar)
{
	m_grammar = std::move(grammar);
	m_lexer = CreateDefaultLexer(*m_grammar);
	m_parser = std::make_unique<Parser>(ParserTable::Create(*m_grammar), *m_lexer);
}

const grammarlib::IGrammar& Language::GetLanguageGrammar() const
{
	if (!m_grammar)
	{
		throw std::logic_error("trying to get grammar before setting it");
	}
	return *m_grammar;
}

IParser<bool>& Language::GetParser()
{
	if (!m_parser)
	{
		throw std::logic_error("trying to get parser before setting grammar");
	}
	return *m_parser;
}

const IParser<bool>& Language::GetParser() const
{
	if (!m_parser)
	{
		throw std::logic_error("trying to get parser before setting grammar");
	}
	return *m_parser;
}

ILexer& Language::GetLexer()
{
	return *m_lexer;
}

const ILexer& Language::GetLexer() const
{
	return *m_lexer;
}
