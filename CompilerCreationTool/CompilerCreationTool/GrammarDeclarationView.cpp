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

GrammarDeclarationView::GrammarDeclarationView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_splitter = new wxSplitterWindow(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxSP_LIVE_UPDATE);
	m_splitter->SetMinimumPaneSize(200);

	CreateLeftPanelControls();
	CreateRightPanelControls();

	m_upTerminalButton->Bind(wxEVT_BUTTON,
		&GrammarDeclarationView::OnTerminalButtonUp, this);
	m_downTerminalButton->Bind(wxEVT_BUTTON,
		&GrammarDeclarationView::OnTerminalButtonDown, this);
	m_editTerminalButton->Bind(wxEVT_BUTTON,
		&GrammarDeclarationView::OnTerminalButtonEdit, this);

	m_terminalsListbox->Bind(wxEVT_LEFT_DOWN,
		&GrammarDeclarationView::OnTerminalsListboxMouseDown, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX_DCLICK,
		&GrammarDeclarationView::OnTerminalsListboxDoubleClick, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_splitter, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);

	SetDoubleBuffered(true);
}

void GrammarDeclarationView::CreateLeftPanelControls()
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

void GrammarDeclarationView::CreateRightPanelControls()
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

wxPanel* GrammarDeclarationView::CreateTerminalsPanel(wxNotebook* notebook)
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

wxPanel* GrammarDeclarationView::CreateActionsPanel(wxNotebook* notebook)
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

wxString GrammarDeclarationView::GetDeclaration() const
{
	return m_input->GetValue();
}

void GrammarDeclarationView::SplitPanels(float sashPositionPercentage)
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

void GrammarDeclarationView::SetLexerTerminals(const ILexer& lexer)
{
	m_terminalsListbox->Clear();
	for (size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		const std::string& name = lexer.GetPattern(i).GetName();
		const unsigned backInsertionIndex = m_terminalsListbox->GetCount();
		m_terminalsListbox->Insert(name, backInsertionIndex);
	}
}

SignalScopedConnection GrammarDeclarationView::DoOnTerminalPositionChange(
	PositionChangeSignal::slot_type slot)
{
	return m_terminalPositionChangeSignal.connect(slot);
}

SignalScopedConnection GrammarDeclarationView::DoOnTerminalEdit(
	TerminalEditSignal::slot_type slot)
{
	return m_terminalEditSignal.connect(slot);
}

SignalScopedConnection GrammarDeclarationView::DoOnGetTerminalPattern(
	GetTerminalPatternSignal::slot_type slot)
{
	return m_getTerminalPatternSignal.connect(slot);
}

void GrammarDeclarationView::OnTerminalButtonUp(wxCommandEvent&)
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

void GrammarDeclarationView::OnTerminalButtonDown(wxCommandEvent&)
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

void GrammarDeclarationView::OnTerminalButtonEdit(wxCommandEvent&)
{
	const int selection = m_terminalsListbox->GetSelection();
	const auto pattern = m_getTerminalPatternSignal(selection);
	assert(pattern);

	if (selection != wxNOT_FOUND)
	{
		if (pattern->IsEnding())
		{
			wxMessageBox(
				"You can't edit grammar's ending terminal.\n"
				"Please, choose another one.",
				"Information about terminal",
				wxOK | wxICON_INFORMATION);
			return;
		}

		wxTextEntryDialog dialog(
			this,
			wxT("Write terminal's regex..."),
			wxT("Edit Terminal"),
			pattern->GetOrigin()
		);

		if (dialog.ShowModal() == wxID_OK)
		{
			wxString newOrigin = dialog.GetValue();
			m_terminalEditSignal(selection, newOrigin);
		}
	}
}

void GrammarDeclarationView::OnTerminalsListboxDoubleClick(wxCommandEvent& event)
{
	const int selection = event.GetInt();
	assert(m_terminalsListbox->GetSelection() == selection);
	assert(selection != wxNOT_FOUND);

	const auto pattern = m_getTerminalPatternSignal(selection);
	assert(pattern);

	if (pattern->IsEnding())
	{
		wxMessageBox(
			"You can't edit grammar's ending terminal.\n"
			"Please, choose another one.",
			"Information about terminal",
			wxOK | wxICON_INFORMATION);
		return;
	}

	wxTextEntryDialog dialog(
		this,
		wxT("Write terminal's regex..."),
		wxT("Edit Terminal"),
		pattern->GetOrigin()
	);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString newOrigin = dialog.GetValue();
		m_terminalEditSignal(selection, newOrigin);
	}
}

void GrammarDeclarationView::OnTerminalsListboxMouseDown(wxMouseEvent& event)
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
