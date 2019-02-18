#include "pch.h"
#include "GrammarPanel.h"

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/notebook.h>

GrammarPanel::GrammarPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_textCtrl(new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP))
{
	m_textCtrl->SetMarginWidth(0, 15);
	m_textCtrl->SetMarginType(0, wxSTC_MARGIN_COLOUR);
	m_textCtrl->SetMarginBackground(0, wxColour(245, 245, 245));

	m_textCtrl->SetMarginWidth(1, 20);
	m_textCtrl->SetMarginLeft(10);
	m_textCtrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 191, 255));
	m_textCtrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(255, 255, 255));
	m_textCtrl->SetMarginType(1, wxSTC_MARGIN_NUMBER);

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

std::string GrammarPanel::GetGrammarText() const
{
	std::cout << m_textCtrl->GetScrollWidth() << std::endl;
	return m_textCtrl->GetValue().ToStdString();
}
