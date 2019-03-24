#include "pch.h"
#include "EntitiesListboxView.h"

EntitiesListboxView::EntitiesListboxView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 120))
	, m_listbox(new wxListBox(this, wxID_ANY))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_listbox, 1, wxEXPAND | wxALL, 5);
	SetSizer(sizer);

	m_listbox->Bind(wxEVT_LISTBOX, &EntitiesListboxView::OnListboxItemSelection, this);
	m_listbox->Bind(wxEVT_LISTBOX_DCLICK, &EntitiesListboxView::OnListboxItemDoubleSelection, this);

	m_listbox->Bind(wxEVT_LEFT_DOWN, &EntitiesListboxView::OnListboxMouseDown, this);
	m_listbox->Bind(wxEVT_SET_FOCUS, &EntitiesListboxView::OnFocusSet, this);
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

void EntitiesListboxView::DeselectAll()
{
	m_listbox->DeselectAll();
}

int EntitiesListboxView::GetSelection() const
{
	return m_listbox->GetSelection();
}

SignalScopedConnection EntitiesListboxView::DoOnItemDoubleSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemDoubleSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnItemSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnFocusSet(Signal<void()>::slot_type slot)
{
	return m_focusSignal.connect(slot);
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
	// Если выбран хотя бы один элемент, а также пользователь кликнул
	//  по пустой части списка, тогда полностью снимаем все выделения
	wxArrayInt selections;
	if (m_listbox->GetSelections(selections) != 0 &&
		m_listbox->HitTest(event.GetPosition()) == wxNOT_FOUND)
	{
		m_listbox->Deselect(wxNOT_FOUND);
	}
	event.Skip(true);
}

void EntitiesListboxView::OnFocusSet(wxFocusEvent& event)
{
	m_focusSignal();
	event.Skip(true);
}
