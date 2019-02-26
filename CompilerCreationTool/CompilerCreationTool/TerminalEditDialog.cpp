#include "pch.h"
#include "TerminalEditDialog.h"
#include <wx/statline.h>

TerminalEditDialog::TerminalEditDialog(const TokenPattern& pattern)
	: wxDialog(nullptr, wxID_ANY, wxT("Edit Terminal"),
		wxDefaultPosition, wxSize(250, 230), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
	, m_pattern(pattern)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxRadioButton* customRegexButton = new wxRadioButton(
		panel, wxID_ANY, "Write your regular expression:");
	m_input = new wxTextCtrl(panel, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize);
	wxStaticLine* line1 = new wxStaticLine(panel);

	sizer->Add(customRegexButton, 0, wxEXPAND | wxALL, 5);
	sizer->Add(m_input, 0, wxEXPAND | wxALL, 5);
	sizer->Add(line1, 0, wxEXPAND | wxALL, 5);

	customRegexButton->Bind(wxEVT_RADIOBUTTON,
		&TerminalEditDialog::OnWriteOwnRegexRadioButtonClick, this);

	wxRadioButton* predefinedRegexButton = new wxRadioButton(
		panel, wxID_ANY, "Choose predefined regular expression:");
	m_combo = new wxComboBox(panel, wxID_ANY, wxT("Choose regex..."),
		wxDefaultPosition, wxDefaultSize, wxArrayString(), wxCB_READONLY);
	wxStaticLine* line2 = new wxStaticLine(panel);

	predefinedRegexButton->Bind(wxEVT_RADIOBUTTON,
		&TerminalEditDialog::OnChoosePredefinedRegexRadioButtonClick, this);

	for (const TokenPattern& defaultPattern : GetPredefinedPatterns())
	{
		m_combo->AppendString(defaultPattern.GetName());
	}
	m_combo->SetSelection(0);

	if (pattern.GetPredefinedIndex() != -1)
	{
		predefinedRegexButton->SetValue(true);
		m_input->Enable(false);
		m_combo->Enable(true);
		m_combo->SetSelection(pattern.GetPredefinedIndex());
	}
	else
	{
		customRegexButton->SetValue(true);
		m_input->Enable(true);
		m_combo->Enable(false);
		m_input->SetValue(pattern.GetOrigin());
	}

	sizer->Add(predefinedRegexButton, 0, wxEXPAND | wxALL, 5);
	sizer->Add(m_combo, 0, wxEXPAND | wxALL, 5);
	sizer->Add(line2, 0, wxEXPAND | wxALL, 5);

	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* closeButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(okButton, 0, wxEXPAND);
	hSizer->Add(closeButton, 0, wxEXPAND | wxLEFT, 5);

	sizer->Add(hSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	panel->SetSizerAndFit(sizer);
}

wxComboBox* TerminalEditDialog::GetComboBox()
{
	return m_combo;
}

wxTextCtrl* TerminalEditDialog::GetTextCtrl()
{
	return m_input;
}

void TerminalEditDialog::OnWriteOwnRegexRadioButtonClick(wxCommandEvent&)
{
	m_input->Enable(true);
	m_combo->Enable(false);
}

void TerminalEditDialog::OnChoosePredefinedRegexRadioButtonClick(wxCommandEvent&)
{
	m_input->Enable(false);
	m_combo->Enable(true);
}
