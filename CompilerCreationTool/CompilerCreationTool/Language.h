#pragma once
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Parser/ParseResults.h"
#include "../Grammar/IGrammar.h"
#include "LanguageInfo.h"

class Language
{
public:
	Language();

	void Reset();
	bool IsInitialized() const;

	void SetGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar);
	const grammarlib::IGrammar& GetGrammar() const;

	IParser<ParseResults>& GetParser();
	const IParser<ParseResults>& GetParser() const;

	ILexer& GetLexer();
	const ILexer& GetLexer() const;

	const LanguageInfo& GetInfo() const;

private:
	std::unique_ptr<ILexer> m_lexer;
	std::unique_ptr<IParser<ParseResults>> m_parser;
	std::unique_ptr<grammarlib::IGrammar> m_grammar;
	std::unique_ptr<LanguageInfo> m_info;
};
