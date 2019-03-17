#pragma once
#include "Language.h"
#include "MainFrame.h"

class LanguageController
{
public:
	LanguageController(Language* language, MainFrame* frame);

private:
	void UpdateStatusbarTerminalInfo(int index);
	void UpdateStatusbarActionInfo(int index);

	// Signal handlers //
	void OnLanguageBuildButtonPress();
	void OnParserRunButtonPress();
	void OnLanguageInfoButtonPress();

	void OnTerminalPositionChange(int oldPos, int newPos);
	void OnTerminalSelection(int selection);
	void OnTerminalEdit(int index);

	void OnActionPositionChange(int oldPos, int newPos);
	void OnActionSelection(int selection);
	void OnActionEdit(int index);

	void OnDeclarationTextCtrlCursorUpdate(int line, int col, int ch);
	void OnEditorTextCtrlCursorUpdate(int line, int col, int ch);
	void OnCursorUpdate(int line, int col, int ch);
	//////////////////////////////////////////////////////////////////////////

private:
	Language* m_language;
	MainFrame* m_frame;
	TreeView* mTreeView;
	EditorView* m_editorView;
	StatesView* m_statesView;
	DeclarationView* m_declarationView;
	std::vector<SignalScopedConnection> m_connections;
};
