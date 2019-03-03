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
	void OnTerminalEdit(int index);

	void OnActionPositionChange(int oldPos, int newPos);
	void OnActionEdit(int index);

private:
	Language* m_language;
	MainFrame* m_frame;
	EditorView* m_editorView;
	StatesView* m_statesView;
	DeclarationView* m_declarationView;
	std::vector<SignalScopedConnection> m_connections;
};
