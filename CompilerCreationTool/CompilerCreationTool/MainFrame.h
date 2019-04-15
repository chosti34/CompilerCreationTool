#pragma once
#include "Signal.h"
#include "ViewIdentifiers.h"

#include "TreeView.h"
#include "TextView.h"
#include "OutputView.h"
#include "StatesView.h"
#include "EntitiesListboxView.h"

#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <map>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxSize& size);
	~MainFrame();

	TextView* GetDeclarationView();
	StatesView* GetStatesView();
	TextView* GetEditorView();
	TreeView* GetTreeView();

	EntitiesListboxView* GetTerminalsView();
	EntitiesListboxView* GetActionsView();
	OutputView* GetOutputView();

	wxStatusBar* GetStatusBar();
	wxToolBar* GetToolBar();

	using ButtonPressSignal = Signal<void()>;
	SignalScopedConnection DoOnButtonPress(Buttons::ID button, ButtonPressSignal::slot_type slot);

private:
	void SendButtonPressedSignal(Buttons::ID buttonID);
	void OpenAboutDialog();

private:
	wxDECLARE_EVENT_TABLE();
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);

	void OnBuild(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnInfo(wxCommandEvent& event);

	void OnItemUp(wxCommandEvent& event);
	void OnItemDown(wxCommandEvent& event);
	void OnItemEdit(wxCommandEvent& event);

	void OnHelp(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

private:
	wxAuiManager mAuiManager;

	wxAuiNotebook* mNotebook;
	TextView* mGrammarView;
	TextView* mEditorView;
	TreeView* mTreeView;
	StatesView* mStatesView;

	EntitiesListboxView* mTerminalsView;
	EntitiesListboxView* mActionsView;
	OutputView* mOutputView;

	wxStatusBar* mStatusbar;
	wxToolBar* mToolbar;

	std::map<Buttons::ID, ButtonPressSignal> mSignals;
};
