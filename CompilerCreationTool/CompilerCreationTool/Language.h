#pragma once
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Parser/ParseResults.h"
#include "../Grammar/IGrammar.h"
#include "LanguageInformation.h"

class Language
{
public:
	bool IsInitialized() const;

	void SetGrammar(std::unique_ptr<grammarlib::IGrammar> && grammar);
	const grammarlib::IGrammar& GetGrammar() const;

	IParser<ParseResults>& GetParser();
	const IParser<ParseResults>& GetParser() const;

	ILexer& GetLexer();
	const ILexer& GetLexer() const;

	const LanguageInformation& GetInfo() const;

private:
	std::unique_ptr<ILexer> m_lexer = nullptr;
	std::unique_ptr<IParser<ParseResults>> m_parser = nullptr;
	std::unique_ptr<grammarlib::IGrammar> m_grammar = nullptr;
	std::unique_ptr<LanguageInformation> m_information;
};
