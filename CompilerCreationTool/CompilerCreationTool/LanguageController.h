#pragma once
#include "Language.h"
#include "MainFrame.h"

class LanguageController
{
public:
	LanguageController(Language* language, MainFrame* frame);

private:
	void OnLanguageBuildButtonPress();
	void OnParserRunButtonPress();
	void OnTerminalPositionChange(int oldPos, int newPos);
	void OnTerminalEdit(int index, const std::string& newPattern);
	const TokenPattern& OnGetTerminalPattern(int index);

private:
	Language* m_language;
	MainFrame* m_frame;
	CodeEditorView* m_editorView;
	ParserStatesView* m_statesView;
	GrammarDeclarationView* m_declarationView;
	std::vector<SignalScopedConnection> m_connections;
};
