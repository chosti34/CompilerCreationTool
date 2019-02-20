#include "pch.h"
#include "GrammarPanel.h"

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/notebook.h>

namespace
{
// Setting up margins and line numbers of styled text control
void ConfigureStyledTextControl(wxStyledTextCtrl& ctrl)
{
	const int cLeftMarginIndex = 0;
	const int cRightMarginIndex = 1;

	ctrl.SetMarginWidth(cLeftMarginIndex, 15);
	ctrl.SetMarginType(cLeftMarginIndex, wxSTC_MARGIN_COLOUR);
	ctrl.SetMarginBackground(cLeftMarginIndex, wxColour(245, 245, 245));

	ctrl.SetMarginWidth(cRightMarginIndex, 20);
	ctrl.StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 191, 255));
	ctrl.StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(255, 255, 255));
	ctrl.SetMarginType(cRightMarginIndex, wxSTC_MARGIN_NUMBER);

	ctrl.SetMarginLeft(10);
}
}

GrammarPanel::GrammarPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	mSplitter = new wxSplitterWindow(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxSP_LIVE_UPDATE);
	mSplitter->SetMinimumPaneSize(150);

	mLeftTextControlPanel = new wxPanel(mSplitter, wxID_ANY);
	mTextControl = new wxStyledTextCtrl(
		mLeftTextControlPanel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	ConfigureStyledTextControl(*mTextControl);

	wxBoxSizer* leftTextControlPanelSizer = new wxBoxSizer(wxVERTICAL);
	leftTextControlPanelSizer->Add(mTextControl, 1, wxEXPAND | wxALL, 5);
	mLeftTextControlPanel->SetSizer(leftTextControlPanelSizer);

	mRightNotebookPanel = new wxPanel(mSplitter, wxID_ANY);
	wxNotebook* notebook = new wxNotebook(
		mRightNotebookPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer* rightNotebookPanelSizer = new wxBoxSizer(wxVERTICAL);
	rightNotebookPanelSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
	mRightNotebookPanel->SetSizer(rightNotebookPanelSizer);

	wxPanel* terminalsPanel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* terminalsPanelSizer = new wxBoxSizer(wxVERTICAL);

	m_terminals = new wxListBox(
		terminalsPanel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	terminalsPanelSizer->Add(m_terminals, 1, wxEXPAND | wxALL, 5);

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

	m_actions = new wxListBox(
		actionsPanel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	actionsPanelSizer->Add(m_actions, 1, wxEXPAND | wxALL, 5);

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

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(mSplitter, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(horizontalSizer);

	SetDoubleBuffered(true);
}

std::string GrammarPanel::GetGrammarText() const
{
	return mTextControl->GetValue().ToStdString();
}

void GrammarPanel::Split()
{
	const int cWidth = GetSize().GetWidth();
	const int cSashPosition = cWidth - int(0.35 * cWidth);

	mSplitter->SplitVertically(
		mLeftTextControlPanel,
		mRightNotebookPanel,
		cSashPosition);
}

wxListBox* GrammarPanel::GetTerminalsListBox()
{
	return m_terminals;
}

const wxListBox* GrammarPanel::GetTerminalsListBox() const
{
	return m_terminals;
}

wxListBox* GrammarPanel::GetActionsListBox()
{
	return m_actions;
}

const wxListBox* GrammarPanel::GetActionsListBox() const
{
	return m_actions;
}
