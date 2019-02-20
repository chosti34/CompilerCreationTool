#pragma once
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Grammar/IGrammar.h"

class Compiler
{
public:
	void SetLanguageGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar);
	const grammarlib::IGrammar& GetLanguageGrammar() const;

	IParser<bool>& GetParser();
	const IParser<bool>& GetParser() const;

	ILexer& GetLexer();
	const ILexer& GetLexer() const;

private:
	std::unique_ptr<ILexer> m_lexer;
	std::unique_ptr<IParser<bool>> m_parser;
	std::unique_ptr<grammarlib::IGrammar> m_grammar;
};
