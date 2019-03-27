#pragma once
#include "Signal.h"
#include "ViewIdentifiers.h"

#include "TreeView.h"
#include "OutputView.h"
#include "EditorView.h"
#include "StatesView.h"
#include "GrammarView.h"
#include "EntitiesListboxView.h"

#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <map>

class MainFrame : public wxFrame
{
public:
	using ButtonPressSignal = Signal<void()>;

	MainFrame(const wxString& title, const wxSize& size);
	~MainFrame();

	GrammarView* GetDeclarationView();
	StatesView* GetStatesView();
	EditorView* GetEditorView();
	TreeView* GetTreeView();

	EntitiesListboxView* GetTerminalsView();
	EntitiesListboxView* GetActionsView();
	OutputView* GetOutputView();

	wxStatusBar* GetStatusBar();
	wxToolBar* GetToolBar();

	SignalScopedConnection DoOnButtonPress(Buttons::ID button,
		ButtonPressSignal::slot_type slot);

private:
	void InvokeSignal(Buttons::ID id);
	void OpenAboutDialog();

private:
	wxDECLARE_EVENT_TABLE();
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnItemUp(wxCommandEvent& event);
	void OnItemDown(wxCommandEvent& event);
	void OnItemEdit(wxCommandEvent& event);

	void OnBuild(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnInfo(wxCommandEvent& event);

	void OnHelp(wxCommandEvent& event);

private:
	wxAuiManager m_auiManager;

	wxAuiNotebook* m_notebook;
	GrammarView* m_declarationView;
	StatesView* m_statesView;
	EditorView* m_editorView;
	TreeView* m_treeView;
	EntitiesListboxView* m_terminalsView;
	EntitiesListboxView* m_actionsView;
	OutputView* m_outputView;

	wxStatusBar* m_statusbar;
	wxToolBar* m_toolbar;

	std::map<Buttons::ID, ButtonPressSignal> m_signals;
};
