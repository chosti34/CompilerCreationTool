#include "pch.h"
#include "EntitiesListboxView.h"

EntitiesListboxView::EntitiesListboxView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 120))
	, m_listbox(new wxListBox(this, wxID_ANY))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_listbox, 1, wxEXPAND | wxALL, 3);
	SetSizer(sizer);

	m_listbox->Bind(wxEVT_LISTBOX, &EntitiesListboxView::OnListboxItemSelection, this);
	m_listbox->Bind(wxEVT_LISTBOX_DCLICK, &EntitiesListboxView::OnListboxItemDoubleSelection, this);
	m_listbox->Bind(wxEVT_LEFT_DOWN, &EntitiesListboxView::OnListboxMouseDown, this);
}

void EntitiesListboxView::SetItems(const wxArrayString& items)
{
	m_listbox->Clear();
	for (const wxString& item : items)
	{
		const unsigned backInsertionIndex = m_listbox->GetCount();
		m_listbox->Insert(item, backInsertionIndex);
	}
}

void EntitiesListboxView::ClearItems()
{
	m_listbox->Clear();
}

void EntitiesListboxView::DeselectAll()
{
	m_listbox->DeselectAll();
}

int EntitiesListboxView::GetSelection() const
{
	return m_listbox->GetSelection();
}

bool EntitiesListboxView::HasSelection() const
{
	return m_listbox->GetSelection() != wxNOT_FOUND;
}

bool EntitiesListboxView::MoveSelectionUp()
{
	const int selection = m_listbox->GetSelection();
	const int upper = selection - 1;
	assert(selection != wxNOT_FOUND);

	if (upper >= 0)
	{
		const wxString swapValue = m_listbox->GetString(selection);
		m_listbox->SetString(selection, m_listbox->GetString(upper));
		m_listbox->SetString(upper, swapValue);
		m_listbox->SetSelection(upper);
		return true;
	}

	return false;
}

bool EntitiesListboxView::MoveSelectionDown()
{
	const int selection = m_listbox->GetSelection();
	const int lower = selection + 1;
	assert(selection != wxNOT_FOUND);

	if (lower < int(m_listbox->GetCount()))
	{
		const wxString swapValue = m_listbox->GetString(selection);
		m_listbox->SetString(selection, m_listbox->GetString(lower));
		m_listbox->SetString(lower, swapValue);
		m_listbox->SetSelection(lower);
		return true;
	}

	return false;
}

SignalScopedConnection EntitiesListboxView::DoOnItemDoubleSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemDoubleSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnItemSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnItemDeselection(Signal<void()>::slot_type slot)
{
	return m_itemDeselectionSignal.connect(slot);
}

void EntitiesListboxView::OnListboxItemDoubleSelection(wxCommandEvent& event)
{
	const int selection = event.GetInt();
	assert(m_listbox->GetSelection() == selection);
	assert(selection != wxNOT_FOUND);
	m_itemDoubleSelectionSignal(selection);
}

void EntitiesListboxView::OnListboxItemSelection(wxCommandEvent& event)
{
	assert(m_listbox->GetSelection() == event.GetSelection());
	assert(m_listbox->GetSelection() != wxNOT_FOUND);
	m_itemSelectionSignal(event.GetSelection());
}

void EntitiesListboxView::OnListboxMouseDown(wxMouseEvent& event)
{
	// ≈сли пользователь кликнул по пустой части списка, тогда полностью снимаем все выделени€
	wxArrayInt selections;
	if (m_listbox->HitTest(event.GetPosition()) == wxNOT_FOUND)
	{
		m_listbox->Deselect(wxNOT_FOUND);
		m_itemDeselectionSignal();
	}
	event.Skip(true);
}
