#pragma once
#include "fwd.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <string>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);
	std::string GetGrammarText() const;
	void Split();

	wxListBox* GetTerminalsListBox();
	const wxListBox* GetTerminalsListBox() const;

	wxListBox* GetActionsListBox();
	const wxListBox* GetActionsListBox() const;

private:
	wxStyledTextCtrl* mTextControl;
	wxSplitterWindow* mSplitter;
	wxPanel* mLeftTextControlPanel;
	wxPanel* mRightNotebookPanel;
	wxListBox* m_terminals;
	wxListBox* m_actions;
};
