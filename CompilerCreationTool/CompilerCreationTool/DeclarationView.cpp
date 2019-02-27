#include "pch.h"
#include "DeclarationView.h"
#include "TerminalEditDialog.h"

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

	m_terminalsListbox->Bind(wxEVT_LEFT_DOWN,
		&DeclarationView::OnTerminalsListboxMouseDown, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX_DCLICK,
		&DeclarationView::OnTerminalsListboxDoubleClick, this);

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
	ConfigureStyledTextControl(*m_input);

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

void DeclarationView::SetParserActions(const IParser<bool>& parser)
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
	TerminalEditSignal::slot_type slot)
{
	return m_terminalEditSignal.connect(slot);
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
