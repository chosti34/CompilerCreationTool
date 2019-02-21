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
	void OnTerminalButtonUp(wxCommandEvent&);
	void OnTerminalButtonDown(wxCommandEvent&);
	void OnTerminalButtonEdit(wxCommandEvent&);

private:
	wxStyledTextCtrl* mTextControl;
	wxSplitterWindow* mSplitter;
	wxPanel* mLeftTextControlPanel;
	wxPanel* mRightNotebookPanel;
	wxListBox* mTerminalsListbox;
	wxListBox* mActionsListbox;
	wxButton* mUpTerminalButton;
	wxButton* mDownTerminalButton;
	wxButton* mEditTerminalButton;
};
