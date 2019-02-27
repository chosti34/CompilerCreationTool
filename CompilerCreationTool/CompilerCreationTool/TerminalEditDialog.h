#pragma once
#include "../Lexer/TokenPattern.h"
#include <wx/dialog.h>

class TerminalEditDialog : public wxDialog
{
public:
	TerminalEditDialog(wxWindow* parent, const TokenPattern& pattern);

	wxComboBox* GetComboBox();
	wxTextCtrl* GetTextCtrl();

private:
	void OnWriteOwnRegexRadioButtonClick(wxCommandEvent&);
	void OnChoosePredefinedRegexRadioButtonClick(wxCommandEvent&);

private:
	wxComboBox* m_combo;
	wxTextCtrl* m_input;
	const TokenPattern& m_pattern;
};
