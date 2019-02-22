#pragma once
#include "fwd.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <string>

#include <boost/signals2.hpp>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);

	boost::signals2::scoped_connection RegisterOnTerminalPositionChangedCallback(
		boost::signals2::signal<void(int, int)>::slot_type callback
	);

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

	boost::signals2::signal<void(int, int)> m_onTerminalPositionChangedSignal;
};
