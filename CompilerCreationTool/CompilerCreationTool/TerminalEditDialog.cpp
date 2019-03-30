#include "pch.h"
#include "TerminalEditDialog.h"
#include <wx/statline.h>

namespace
{
const wxSize DIALOG_SIZE = { 250, 250 };
}

TerminalEditDialog::TerminalEditDialog(wxWindow* parent, TokenPattern& pattern)
	: wxDialog(parent, wxID_ANY, wxT("Edit Terminal '" + pattern.GetName() + "'"),
		wxDefaultPosition, DIALOG_SIZE, wxDEFAULT_DIALOG_STYLE)
	, m_pattern(pattern)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* box = new wxStaticBox(panel, wxID_ANY, wxT("Write regex"));
	wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(box, wxVERTICAL);

	m_textCtrl = new wxTextCtrl(box, wxID_ANY, pattern.GetOrigin());
	wxStaticLine* line = new wxStaticLine(box, wxID_ANY);
	m_listbox = new wxListBox(box, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString{}, wxLB_SINGLE | wxLB_ALWAYS_SB);

	for (const TokenPattern& predefinedPattern : GetPredefinedPatterns())
	{
		m_listbox->AppendString(predefinedPattern.GetName());
	}

	staticBoxSizer->Add(m_textCtrl, 0, wxEXPAND | wxALL, 5);
	staticBoxSizer->Add(line, 0, wxEXPAND | wxALL, 5);
	staticBoxSizer->Add(m_listbox, 1, wxEXPAND | wxALL, 5);
	sizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 5);

	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* closeButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	okButton->Bind(wxEVT_BUTTON, &TerminalEditDialog::OnOk, this);
	m_listbox->Bind(wxEVT_LISTBOX, &TerminalEditDialog::OnListboxSelection, this);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(okButton, 0, wxEXPAND);
	hSizer->Add(closeButton, 0, wxEXPAND | wxLEFT, 5);

	sizer->Add(hSizer, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 5);
	panel->SetSizerAndFit(sizer);
	CentreOnParent();
}

void TerminalEditDialog::OnOk(wxCommandEvent& event)
{
	const std::string origin = m_textCtrl->GetValue();
	if (!m_pattern.SetOrigin(origin))
	{
		wxMessageBox(
			wxT("Token pattern's regular expression must not allow empty strings!"),
			wxT("Warning"),
			wxICON_WARNING
		);
		return;
	}
	event.Skip();
}

void TerminalEditDialog::OnListboxSelection(wxCommandEvent& event)
{
	const int selection = event.GetSelection();
	assert(selection != wxNOT_FOUND);
	assert(unsigned(selection) < m_listbox->GetCount());

	const std::vector<TokenPattern>& predefinedPatterns = GetPredefinedPatterns();
	const TokenPattern& selected = predefinedPatterns[selection];

	m_textCtrl->SetValue(selected.GetOrigin());
}
