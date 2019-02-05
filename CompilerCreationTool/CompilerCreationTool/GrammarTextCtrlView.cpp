#include "pch.h"
#include "GrammarTextCtrlView.h"

#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

#include <wx/aui/aui.h>

GrammarTextCtrlView::GrammarTextCtrlView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_textCtrl(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_TAB | wxTE_DONTWRAP))
{
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 5);

	wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	hSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

	wxPanel* terminalsPanel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* terminalsPanelSizer = new wxBoxSizer(wxVERTICAL);

	wxListBox* terminalsList = new wxListBox(terminalsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	terminalsList->Insert(wxT("string"), 0);
	terminalsPanelSizer->Add(terminalsList, 1, wxEXPAND | wxALL, 5);

	// Buttons for terminals list
	wxButton* addTerminalButton = new wxButton(terminalsPanel, wxID_ANY, wxT("Add"));
	wxButton* deleteTerminalButton = new wxButton(terminalsPanel, wxID_ANY, wxT("Delete"));

	wxBoxSizer* terminalButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	terminalButtonsSizer->Add(addTerminalButton, 0);
	terminalButtonsSizer->Add(deleteTerminalButton, 0, wxLEFT, 5);

	terminalsPanelSizer->Add(terminalButtonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	terminalsPanel->SetSizer(terminalsPanelSizer);

	wxPanel* actionsPanel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* actionsPanelSizer = new wxBoxSizer(wxVERTICAL);

	wxListBox* actionsList = new wxListBox(actionsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	actionsPanelSizer->Add(actionsList, 1, wxEXPAND | wxALL, 5);

	// Buttons for actions list
	wxButton* addActionButton = new wxButton(actionsPanel, wxID_ANY, wxT("Add"));
	wxButton* deleteActionButton = new wxButton(actionsPanel, wxID_ANY, wxT("Delete"));

	wxBoxSizer* actionButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	actionButtonsSizer->Add(addActionButton, 0);
	actionButtonsSizer->Add(deleteActionButton, 0, wxLEFT, 5);

	actionsPanelSizer->Add(actionButtonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	actionsPanel->SetSizer(actionsPanelSizer);

	notebook->AddPage(terminalsPanel, wxT("Terminals"));
	notebook->AddPage(actionsPanel, wxT("Actions"));

	SetSizerAndFit(hSizer);
}
