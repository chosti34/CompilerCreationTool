#include "pch.h"
#include "GrammarDeclarationView.h"

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/artprov.h>

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

GrammarDeclarationView::GrammarDeclarationView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	mSplitter = new wxSplitterWindow(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxSP_LIVE_UPDATE);
	mSplitter->SetMinimumPaneSize(200);

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

	mTerminalsListbox = new wxListBox(
		terminalsPanel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString());
	terminalsPanelSizer->Add(mTerminalsListbox, 1, wxEXPAND | wxALL, 5);

	// Buttons for terminals list
	mUpTerminalButton = new wxButton(terminalsPanel, wxID_ANY);
	mDownTerminalButton = new wxButton(terminalsPanel, wxID_ANY);
	mEditTerminalButton = new wxButton(terminalsPanel, wxID_ANY);

	mUpTerminalButton->Bind(wxEVT_BUTTON, &GrammarDeclarationView::OnTerminalButtonUp, this);
	mDownTerminalButton->Bind(wxEVT_BUTTON, &GrammarDeclarationView::OnTerminalButtonDown, this);
	mEditTerminalButton->Bind(wxEVT_BUTTON, &GrammarDeclarationView::OnTerminalButtonEdit, this);

	wxBitmap upBitmap = wxArtProvider::GetBitmap(wxART_GO_UP);
	mUpTerminalButton->SetBitmap(upBitmap);
	mUpTerminalButton->SetMaxSize(wxSize(48, 48));

	wxBitmap downBitmap = wxArtProvider::GetBitmap(wxART_GO_DOWN);
	mDownTerminalButton->SetBitmap(downBitmap);
	mDownTerminalButton->SetMaxSize(wxSize(48, 48));

	wxBitmap printBitmap = wxArtProvider::GetBitmap(wxART_PRINT);
	mEditTerminalButton->SetBitmap(printBitmap);
	mEditTerminalButton->SetMaxSize(wxSize(48, 48));

	wxBoxSizer* terminalButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	terminalButtonsSizer->Add(mUpTerminalButton, 0);
	terminalButtonsSizer->Add(mDownTerminalButton, 0, wxLEFT, 5);
	terminalButtonsSizer->Add(mEditTerminalButton, 0, wxLEFT, 5);

	terminalsPanelSizer->Add(terminalButtonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	terminalsPanel->SetSizer(terminalsPanelSizer);

	wxPanel* actionsPanel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* actionsPanelSizer = new wxBoxSizer(wxVERTICAL);

	mActionsListbox = new wxListBox(
		actionsPanel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	actionsPanelSizer->Add(mActionsListbox, 1, wxEXPAND | wxALL, 5);

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

SignalScopedConnection GrammarDeclarationView::DoOnTerminalPositionChange(
	PositionChangeSignal::slot_type slot)
{
	return m_terminalPositionChangeSignal.connect(slot);
}

wxString GrammarDeclarationView::GetDeclaration() const
{
	return mTextControl->GetValue();
}

void GrammarDeclarationView::SplitPanels(float sashPositionPercentage)
{
	assert(sashPositionPercentage <= 1.f);

	const int cWidth = GetSize().GetWidth();
	const int cSashPosition = int(sashPositionPercentage * cWidth);

	mSplitter->SplitVertically(
		mLeftTextControlPanel,
		mRightNotebookPanel,
		cSashPosition
	);
}

void GrammarDeclarationView::SetLexerTerminals(const ILexer& lexer)
{
	mTerminalsListbox->Clear();
	for (size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		const std::string& name = lexer.GetPattern(i).GetName();
		const unsigned backInsertionIndex = mTerminalsListbox->GetCount();
		mTerminalsListbox->Insert(name, backInsertionIndex);
	}
}

void GrammarDeclarationView::OnTerminalButtonUp(wxCommandEvent&)
{
	const int selection = mTerminalsListbox->GetSelection();
	const int upperIndex = selection - 1;

	if (selection != wxNOT_FOUND && upperIndex >= 0)
	{
		m_terminalPositionChangeSignal(selection, upperIndex);
		const wxString swapValue = mTerminalsListbox->GetString(selection);
		mTerminalsListbox->SetString(selection, mTerminalsListbox->GetString(upperIndex));
		mTerminalsListbox->SetString(upperIndex, swapValue);
		mTerminalsListbox->SetSelection(upperIndex);
	}
}

void GrammarDeclarationView::OnTerminalButtonDown(wxCommandEvent&)
{
	const int selection = mTerminalsListbox->GetSelection();
	const unsigned lowerIndex = unsigned(selection) + 1;

	if (selection != wxNOT_FOUND && lowerIndex < mTerminalsListbox->GetCount())
	{
		m_terminalPositionChangeSignal(selection, lowerIndex);
		const wxString swapValue = mTerminalsListbox->GetString(selection);
		mTerminalsListbox->SetString(selection, mTerminalsListbox->GetString(lowerIndex));
		mTerminalsListbox->SetString(lowerIndex, swapValue);
		mTerminalsListbox->SetSelection(lowerIndex);
	}
}

void GrammarDeclarationView::OnTerminalButtonEdit(wxCommandEvent&)
{
	int selection = mTerminalsListbox->GetSelection();
	std::cout << "Terminal button edit" << std::endl;
}
