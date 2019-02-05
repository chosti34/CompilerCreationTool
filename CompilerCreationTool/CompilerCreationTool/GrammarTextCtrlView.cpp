#include "pch.h"
#include "GrammarTextCtrlView.h"

#include <wx/statbox.h>

GrammarTextCtrlView::GrammarTextCtrlView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_textCtrl(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_TAB | wxTE_DONTWRAP))
{
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	hSizer->Add(vSizer, 1, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* stSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Terminals"));
	wxListBox* terminalsList = new wxListBox(stSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxLB_ALWAYS_SB);
	terminalsList->Insert(wxT("string"), 0);
	stSizer1->Add(terminalsList, 1, wxEXPAND | wxALL, 5);

	// Buttons for terminals list
	wxButton* addTerminalButton = new wxButton(stSizer1->GetStaticBox(), wxID_ANY, wxT("Add"));
	wxButton* deleteTerminalButton = new wxButton(stSizer1->GetStaticBox(), wxID_ANY, wxT("Delete"));

	wxBoxSizer* terminalButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	terminalButtonsSizer->Add(addTerminalButton, 0);
	terminalButtonsSizer->Add(deleteTerminalButton, 0);

	stSizer1->Add(terminalButtonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	wxStaticBoxSizer* stSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Actions"));
	wxListBox* actionsList = new wxListBox(stSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxLB_ALWAYS_SB);
	stSizer2->Add(actionsList, 1, wxEXPAND | wxALL, 5);

	// Buttons for actions list
	wxButton* addActionButton = new wxButton(stSizer2->GetStaticBox(), wxID_ANY, wxT("Add"));
	wxButton* deleteActionButton = new wxButton(stSizer2->GetStaticBox(), wxID_ANY, wxT("Delete"));

	wxBoxSizer* actionButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	actionButtonsSizer->Add(addActionButton, 0);
	actionButtonsSizer->Add(deleteActionButton, 0);

	stSizer2->Add(actionButtonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	vSizer->Add(stSizer1, 1, wxEXPAND);
	vSizer->Add(stSizer2, 1, wxEXPAND);
	SetSizerAndFit(hSizer);
}
