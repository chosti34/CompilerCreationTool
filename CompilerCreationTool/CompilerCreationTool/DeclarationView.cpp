#include "pch.h"
#include "DeclarationView.h"
#include "TerminalEditDialog.h"
#include "CtrlHelpers.h"

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/artprov.h>

namespace
{
wxButton* CreateButton(wxPanel* panel, const wxArtID& artId)
{
	wxButton* button = new wxButton(panel, wxID_ANY);
	wxBitmap bitmap = wxArtProvider::GetBitmap(artId);
	button->SetBitmap(bitmap);
	button->SetMinSize(wxSize(42, 32));
	button->SetMaxSize(wxSize(48, 48));
	return button;
}
}

DeclarationView::DeclarationView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_splitter = new wxSplitterWindow(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxSP_LIVE_UPDATE);
	m_splitter->SetMinimumPaneSize(200);

	CreateLeftPanelControls();
	CreateRightPanelControls();

	m_upTerminalButton->Bind(wxEVT_BUTTON,
		&DeclarationView::OnTerminalButtonUp, this);
	m_downTerminalButton->Bind(wxEVT_BUTTON,
		&DeclarationView::OnTerminalButtonDown, this);
	m_editTerminalButton->Bind(wxEVT_BUTTON,
		&DeclarationView::OnTerminalButtonEdit, this);

	m_terminalsListbox->Bind(wxEVT_LEFT_DOWN, &DeclarationView::OnTerminalsListboxMouseDown, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX_DCLICK, &DeclarationView::OnTerminalsListboxDoubleClick, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX, &DeclarationView::OnTerminalsListboxSelection, this);

	m_upActionButton->Bind(wxEVT_BUTTON, &DeclarationView::OnActionButtonUp, this);
	m_downActionButton->Bind(wxEVT_BUTTON, &DeclarationView::OnActionButtonDown, this);
	m_editActionButton->Bind(wxEVT_BUTTON, &DeclarationView::OnActionButtonEdit, this);

	m_actionsListbox->Bind(wxEVT_LEFT_DOWN, &DeclarationView::OnActionsListboxMouseDown, this);
	m_actionsListbox->Bind(wxEVT_LISTBOX_DCLICK, &DeclarationView::OnActionsListboxDoubleClick, this);
	m_actionsListbox->Bind(wxEVT_LISTBOX, &DeclarationView::OnActionsListboxSelection, this);

	m_input->Bind(wxEVT_STC_UPDATEUI, &DeclarationView::OnTextCtrlCursorUpdate, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_splitter, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);

	SetDoubleBuffered(true);
}

void DeclarationView::CreateLeftPanelControls()
{
	m_left = new wxPanel(m_splitter, wxID_ANY);

	m_input = new wxStyledTextCtrl(
		m_left, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	SetupStyledTextCtrlMargins(*m_input);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_input, 1, wxEXPAND | wxALL, 5);
	m_left->SetSizer(sizer);
}

void DeclarationView::CreateRightPanelControls()
{
	m_right = new wxPanel(m_splitter, wxID_ANY);
	wxNotebook* notebook = new wxNotebook(m_right, wxID_ANY);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
	m_right->SetSizer(sizer);

	wxPanel* terminalsPanel = CreateTerminalsPanel(notebook);
	wxPanel* actionsPanel = CreateActionsPanel(notebook);

	notebook->AddPage(terminalsPanel, wxT("Terminals"));
	notebook->AddPage(actionsPanel, wxT("Actions"));
}

wxPanel* DeclarationView::CreateTerminalsPanel(wxNotebook* notebook)
{
	wxPanel* panel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

	m_terminalsListbox = new wxListBox(
		panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxArrayString(), wxBORDER_SIMPLE);
	pSizer->Add(m_terminalsListbox, 1, wxEXPAND | wxALL, 5);

	m_upTerminalButton = CreateButton(panel, wxART_GO_UP);
	m_downTerminalButton = CreateButton(panel, wxART_GO_DOWN);
	m_editTerminalButton = CreateButton(panel, wxART_LIST_VIEW);

	wxBoxSizer* bSizer = new wxBoxSizer(wxHORIZONTAL);
	bSizer->Add(m_upTerminalButton, 0);
	bSizer->Add(m_downTerminalButton, 0, wxLEFT, 5);
	bSizer->Add(m_editTerminalButton, 0, wxLEFT, 5);

	const int buttonsAlignFlags = wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM;
	pSizer->Add(bSizer, 0, buttonsAlignFlags, 5);
	panel->SetSizer(pSizer);

	return panel;
}

wxPanel* DeclarationView::CreateActionsPanel(wxNotebook* notebook)
{
	wxPanel* panel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

	m_actionsListbox = new wxListBox(
		panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE);
	pSizer->Add(m_actionsListbox, 1, wxEXPAND | wxALL, 5);

	m_upActionButton = CreateButton(panel, wxART_GO_UP);
	m_downActionButton = CreateButton(panel, wxART_GO_DOWN);
	m_editActionButton = CreateButton(panel, wxART_LIST_VIEW);

	wxBoxSizer* bSizer = new wxBoxSizer(wxHORIZONTAL);
	bSizer->Add(m_upActionButton, 0);
	bSizer->Add(m_downActionButton, 0, wxLEFT, 5);
	bSizer->Add(m_editActionButton, 0, wxLEFT, 5);

	const int buttonsAlignFlags = wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM;
	pSizer->Add(bSizer, 0, buttonsAlignFlags, 5);
	panel->SetSizer(pSizer);

	return panel;
}

wxString DeclarationView::GetDeclaration() const
{
	return m_input->GetValue();
}

void DeclarationView::SplitPanels(float sashPositionPercentage)
{
	assert(sashPositionPercentage <= 1.f);
	const int cWidth = GetSize().GetWidth();
	const int cSashPosition = int(sashPositionPercentage * cWidth);

	m_splitter->SplitVertically(
		m_left,
		m_right,
		cSashPosition
	);
}

SignalScopedConnection DeclarationView::DoOnTextCtrlCursorUpdate(CursorUpdateSignal::slot_type slot)
{
	return mTextCtrlCursorUpdateSignal.connect(slot);
}

void DeclarationView::SetLexerTerminals(const ILexer& lexer)
{
	m_terminalsListbox->Clear();
	for (size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		const std::string& name = lexer.GetPattern(i).GetName();
		const unsigned backInsertionIndex = m_terminalsListbox->GetCount();
		m_terminalsListbox->Insert(name, backInsertionIndex);
	}
}

void DeclarationView::SetParserActions(const IParser<ParseResults>& parser)
{
	m_actionsListbox->Clear();
	for (size_t i = 0; i < parser.GetActionsCount(); ++i)
	{
		const std::string& name = parser.GetAction(i).GetName();
		const unsigned backInsertionIndex = m_actionsListbox->GetCount();
		m_actionsListbox->Insert(name, backInsertionIndex);
	}
}

SignalScopedConnection DeclarationView::DoOnTerminalPositionChange(
	PositionChangeSignal::slot_type slot)
{
	return m_terminalPositionChangeSignal.connect(slot);
}

SignalScopedConnection DeclarationView::DoOnTerminalEdit(
	ItemEditSignal::slot_type slot)
{
	return m_terminalEditSignal.connect(slot);
}

SignalScopedConnection DeclarationView::DoOnTerminalSelection(SelectionSignal::slot_type slot)
{
	return mTerminalSelectionSignal.connect(slot);
}

SignalScopedConnection DeclarationView::DoOnActionPositionChange(
	PositionChangeSignal::slot_type slot)
{
	return m_actionPositionChangeSignal.connect(slot);
}

SignalScopedConnection DeclarationView::DoOnActionEdit(
	ItemEditSignal::slot_type slot)
{
	return m_actionEditSignal.connect(slot);
}

SignalScopedConnection DeclarationView::DoOnActionSelection(SelectionSignal::slot_type slot)
{
	return mActionSelectionSignal.connect(slot);
}

void DeclarationView::OnTerminalButtonUp(wxCommandEvent&)
{
	const int selection = m_terminalsListbox->GetSelection();
	const int upperIndex = selection - 1;

	if (selection != wxNOT_FOUND && upperIndex >= 0)
	{
		m_terminalPositionChangeSignal(selection, upperIndex);
		const wxString swapValue = m_terminalsListbox->GetString(selection);
		m_terminalsListbox->SetString(selection, m_terminalsListbox->GetString(upperIndex));
		m_terminalsListbox->SetString(upperIndex, swapValue);
		m_terminalsListbox->SetSelection(upperIndex);
	}
}

void DeclarationView::OnTerminalButtonDown(wxCommandEvent&)
{
	const int selection = m_terminalsListbox->GetSelection();
	const unsigned lowerIndex = unsigned(selection) + 1;

	if (selection != wxNOT_FOUND && lowerIndex < m_terminalsListbox->GetCount())
	{
		m_terminalPositionChangeSignal(selection, lowerIndex);
		const wxString swapValue = m_terminalsListbox->GetString(selection);
		m_terminalsListbox->SetString(selection, m_terminalsListbox->GetString(lowerIndex));
		m_terminalsListbox->SetString(lowerIndex, swapValue);
		m_terminalsListbox->SetSelection(lowerIndex);
	}
}

void DeclarationView::OnTerminalButtonEdit(wxCommandEvent&)
{
	const int selection = m_terminalsListbox->GetSelection();
	if (selection != wxNOT_FOUND)
	{
		m_terminalEditSignal(selection);
	}
}

void DeclarationView::OnTerminalsListboxDoubleClick(wxCommandEvent& event)
{
	const int selection = event.GetInt();
	assert(m_terminalsListbox->GetSelection() == selection);
	assert(selection != wxNOT_FOUND);
	m_terminalEditSignal(selection);
}

void DeclarationView::OnTerminalsListboxMouseDown(wxMouseEvent& event)
{
	// Если выбран хотя бы один элемент, а также пользователь кликнул
	//  по пустой части списка, тогда полностью снимаем все выделения
	wxArrayInt selections;
	if (m_terminalsListbox->GetSelections(selections) != 0 &&
		m_terminalsListbox->HitTest(event.GetPosition()) == wxNOT_FOUND)
	{
		m_terminalsListbox->Deselect(wxNOT_FOUND);
	}
	event.Skip(true);
}

void DeclarationView::OnTerminalsListboxSelection(wxCommandEvent& event)
{
	assert(m_terminalsListbox->GetSelection() == event.GetSelection());
	assert(m_terminalsListbox->GetSelection() != wxNOT_FOUND);
	mTerminalSelectionSignal(event.GetSelection());
}

void DeclarationView::OnActionButtonUp(wxCommandEvent&)
{
	const int selection = m_actionsListbox->GetSelection();
	const int upperIndex = selection - 1;

	if (selection != wxNOT_FOUND && upperIndex >= 0)
	{
		m_actionPositionChangeSignal(selection, upperIndex);
		const wxString swapValue = m_actionsListbox->GetString(selection);
		m_actionsListbox->SetString(selection, m_actionsListbox->GetString(upperIndex));
		m_actionsListbox->SetString(upperIndex, swapValue);
		m_actionsListbox->SetSelection(upperIndex);
	}
}

void DeclarationView::OnActionButtonDown(wxCommandEvent&)
{
	const int selection = m_actionsListbox->GetSelection();
	const unsigned lowerIndex = unsigned(selection) + 1;

	if (selection != wxNOT_FOUND && lowerIndex < m_actionsListbox->GetCount())
	{
		m_actionPositionChangeSignal(selection, lowerIndex);
		const wxString swapValue = m_actionsListbox->GetString(selection);
		m_actionsListbox->SetString(selection, m_actionsListbox->GetString(lowerIndex));
		m_actionsListbox->SetString(lowerIndex, swapValue);
		m_actionsListbox->SetSelection(lowerIndex);
	}
}

void DeclarationView::OnActionButtonEdit(wxCommandEvent&)
{
	const int selection = m_actionsListbox->GetSelection();
	if (selection != wxNOT_FOUND)
	{
		m_actionEditSignal(selection);
	}
}

void DeclarationView::OnActionsListboxDoubleClick(wxCommandEvent& event)
{
	const int selection = event.GetInt();
	assert(m_actionsListbox->GetSelection() == selection);
	assert(selection != wxNOT_FOUND);
	m_actionEditSignal(selection);
}

void DeclarationView::OnActionsListboxMouseDown(wxMouseEvent& event)
{
	// Если выбран хотя бы один элемент, а также пользователь кликнул
	//  по пустой части списка, тогда полностью снимаем все выделения
	wxArrayInt selections;
	if (m_actionsListbox->GetSelections(selections) != 0 &&
		m_actionsListbox->HitTest(event.GetPosition()) == wxNOT_FOUND)
	{
		m_actionsListbox->Deselect(wxNOT_FOUND);
	}
	event.Skip(true);
}

void DeclarationView::OnActionsListboxSelection(wxCommandEvent& event)
{
	assert(m_actionsListbox->GetSelection() == event.GetSelection());
	assert(m_actionsListbox->GetSelection() != wxNOT_FOUND);
	mActionSelectionSignal(event.GetSelection());
}

void DeclarationView::OnTextCtrlCursorUpdate(wxStyledTextEvent& event)
{
	int linePos = 0;
	m_input->GetCurLine(&linePos);

	mTextCtrlCursorUpdateSignal(
		m_input->GetCurrentLine() + 1,
		m_input->GetColumn(m_input->GetCurrentPos()) + 1,
		linePos + 1
	);

	event.Skip(true);
}
