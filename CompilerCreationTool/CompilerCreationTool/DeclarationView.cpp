#include "pch.h"
#include "DeclarationView.h"
#include "TerminalEditDialog.h"
#include "CtrlHelpers.h"

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/artprov.h>

#include <wx/listctrl.h>
#include <wx/grid.h>
#include <wx/aui/auibook.h>

namespace
{
wxButton* CreateButton(wxWindow* panel, const wxArtID& artId)
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
	wxStaticBoxSizer* leftStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Grammar");
	m_input = new wxStyledTextCtrl(leftStaticBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP);
	leftStaticBoxSizer->Add(m_input, 1, wxEXPAND | wxALL, 5);

	m_input->SetScrollWidth(m_input->GetSize().GetWidth());
	m_input->SetScrollWidthTracking(true);
	SetupStyledTextCtrlMargins(*m_input);

	wxStaticBoxSizer* terminalsStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Terminals");
	m_terminalsListbox = new wxListBox(
		this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxArrayString(), wxBORDER_SIMPLE
	);
	terminalsStaticBoxSizer->Add(m_terminalsListbox, 1, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* actionsStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Actions");
	m_actionsListbox = new wxListBox(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxArrayString(), wxBORDER_SIMPLE
	);
	actionsStaticBoxSizer->Add(m_actionsListbox, 1, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* buttonsStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this);
	mUpButton = CreateButton(buttonsStaticBoxSizer->GetStaticBox(), wxART_GO_UP);
	mDownButton = CreateButton(buttonsStaticBoxSizer->GetStaticBox(), wxART_GO_DOWN);
	mEditButton = CreateButton(buttonsStaticBoxSizer->GetStaticBox(), wxART_LIST_VIEW);

	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonsSizer->Add(mUpButton, 0, wxEXPAND | wxALL, 3);
	buttonsSizer->Add(mDownButton, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 3);
	buttonsSizer->Add(mEditButton, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 3);
	buttonsStaticBoxSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT);

	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
	rightSizer->Add(terminalsStaticBoxSizer, 1, wxEXPAND | wxALL, 5);
	rightSizer->Add(actionsStaticBoxSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
	rightSizer->Add(buttonsStaticBoxSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

	m_terminalsListbox->Bind(wxEVT_LEFT_DOWN, &DeclarationView::OnTerminalsListboxMouseDown, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX_DCLICK, &DeclarationView::OnTerminalsListboxDoubleClick, this);
	m_terminalsListbox->Bind(wxEVT_LISTBOX, &DeclarationView::OnTerminalsListboxSelection, this);

	m_actionsListbox->Bind(wxEVT_LEFT_DOWN, &DeclarationView::OnActionsListboxMouseDown, this);
	m_actionsListbox->Bind(wxEVT_LISTBOX_DCLICK, &DeclarationView::OnActionsListboxDoubleClick, this);
	m_actionsListbox->Bind(wxEVT_LISTBOX, &DeclarationView::OnActionsListboxSelection, this);

	m_input->Bind(wxEVT_STC_UPDATEUI, &DeclarationView::OnTextCtrlCursorUpdate, this);

	mUpButton->Bind(wxEVT_BUTTON, &DeclarationView::OnUpButton, this);
	mDownButton->Bind(wxEVT_BUTTON, &DeclarationView::OnDownButton, this);
	mEditButton->Bind(wxEVT_BUTTON, &DeclarationView::OnEditButton, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(leftStaticBoxSizer, 3, wxEXPAND | wxALL, 5);
	sizer->Add(rightSizer, 2, wxEXPAND);
	SetSizerAndFit(sizer);

	SetDoubleBuffered(true);
}

wxString DeclarationView::GetDeclaration() const
{
	return m_input->GetValue();
}

SignalScopedConnection DeclarationView::DoOnTextCtrlCursorUpdate(CursorUpdateSignal::slot_type slot)
{
	return mCursorUpdateSignal.connect(slot);
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

void DeclarationView::OnTerminalButtonUp()
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

void DeclarationView::OnTerminalButtonDown()
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

void DeclarationView::OnTerminalButtonEdit()
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
	m_actionsListbox->Deselect(wxNOT_FOUND);
	// ���� ������ ���� �� ���� �������, � ����� ������������ �������
	//  �� ������ ����� ������, ����� ��������� ������� ��� ���������
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

void DeclarationView::OnActionButtonUp()
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

void DeclarationView::OnActionButtonDown()
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

void DeclarationView::OnActionButtonEdit()
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
	m_terminalsListbox->Deselect(wxNOT_FOUND);
	// ���� ������ ���� �� ���� �������, � ����� ������������ �������
	//  �� ������ ����� ������, ����� ��������� ������� ��� ���������
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

void DeclarationView::OnUpButton(wxCommandEvent&)
{
	if (m_terminalsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_actionsListbox->GetSelection() == wxNOT_FOUND);
		OnTerminalButtonUp();
		return;
	}

	if (m_actionsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_terminalsListbox->GetSelection() == wxNOT_FOUND);
		OnActionButtonUp();
		return;
	}
}

void DeclarationView::OnDownButton(wxCommandEvent&)
{
	if (m_terminalsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_actionsListbox->GetSelection() == wxNOT_FOUND);
		OnTerminalButtonDown();
		return;
	}

	if (m_actionsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_terminalsListbox->GetSelection() == wxNOT_FOUND);
		OnActionButtonDown();
		return;
	}
}

void DeclarationView::OnEditButton(wxCommandEvent&)
{
	if (m_terminalsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_actionsListbox->GetSelection() == wxNOT_FOUND);
		OnTerminalButtonEdit();
		return;
	}

	if (m_actionsListbox->GetSelection() != wxNOT_FOUND)
	{
		assert(m_terminalsListbox->GetSelection() == wxNOT_FOUND);
		OnActionButtonEdit();
		return;
	}
}

void DeclarationView::OnTextCtrlCursorUpdate(wxStyledTextEvent& event)
{
	int linePos = 0;
	m_input->GetCurLine(&linePos);

	mCursorUpdateSignal(
		m_input->GetCurrentLine() + 1,
		m_input->GetColumn(m_input->GetCurrentPos()) + 1,
		linePos + 1
	);

	event.Skip(true);
}
