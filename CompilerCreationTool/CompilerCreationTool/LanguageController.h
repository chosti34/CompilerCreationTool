#pragma once
#include "Language.h"
#include "MainFrame.h"

class LanguageController
{
public:
	LanguageController(Language* language, MainFrame* frame);

private:
	void UpdateStatusBarOnTextFocusChange(TextView& view, const std::string& name, bool focus);
	void UpdateStatusBarCursorInfo(int line, int col, int ch);
	void SwapTerminalPositions(int from, int to);
	void SwapActionPositions(int from, int to);
	void UpdateTitle();

private: // Signal handlers
	void OnNewButtonPress();
	void OnOpenButtonPress();
	void OnSaveButtonPress();
	void OnSaveAsButtonPress();
	void OnClearButtonPress();
	void OnLogMessageButtonPress();
	void OnEnableCodegenButtonPress();

	void OnBuildButtonPress();
	void OnRunButtonPress();
	void OnInfoButtonPress();

	void OnUpButtonPress();
	void OnDownButtonPress();
	void OnEditButtonPress();

	void OnTerminalsViewDeselection();
	void OnActionsViewDeselection();

	void OnTerminalSelection(int selection);
	void OnActionSelection(int selection);

	void OnTerminalEdit(int index);
	void OnActionEdit(int index);

	void OnGrammarTextCtrlUpdateUI(int line, int col, int ch);
	void OnEditorTextCtrlUpdateUI(int line, int col, int ch);
	void OnGrammarTextFocusChange(bool focus);
	void OnEditorTextFocusChange(bool focus);

private:
	Language* mLanguage;
	MainFrame* mFrame;

	TextView* mGrammarView;
	StatesView* mStatesView;
	TextView* mEditorView;
	TreeView* mTreeView;

	EntitiesListboxView* mTerminalsView;
	EntitiesListboxView* mActionsView;
	OutputView* mOutputView;

	std::vector<SignalScopedConnection> mConnections;
	boost::optional<std::string> mDocument;
	bool mHasUnsavedChanges;
	bool mNeedCodegen;
};
