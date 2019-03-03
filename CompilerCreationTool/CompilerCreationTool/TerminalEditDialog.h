#pragma once
#include "../Lexer/TokenPattern.h"
#include <wx/dialog.h>

class TerminalEditDialog : public wxDialog
{
public:
	TerminalEditDialog(wxWindow* parent, TokenPattern& pattern);

private:
	void OnOk(wxCommandEvent& event);
	void OnListboxSelection(wxCommandEvent& event);

private:
	wxTextCtrl* m_textCtrl;
	wxListBox* m_listbox;
	TokenPattern& m_pattern;
};
