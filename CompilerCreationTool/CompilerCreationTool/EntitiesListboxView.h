#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/listbox.h>

class EntitiesListboxView : public wxPanel
{
public:
	explicit EntitiesListboxView(wxWindow* parent);

	void SetItems(const wxArrayString& items);
	void ClearItems();
	void DeselectAll();

	int GetSelection() const;
	bool HasSelection() const;

	bool MoveSelectionUp();
	bool MoveSelectionDown();

	SignalScopedConnection DoOnItemDoubleSelection(Signal<void(int)>::slot_type);
	SignalScopedConnection DoOnItemSelection(Signal<void(int)>::slot_type);
	SignalScopedConnection DoOnItemDeselection(Signal<void()>::slot_type);

private:
	void OnListboxItemDoubleSelection(wxCommandEvent& event);
	void OnListboxItemSelection(wxCommandEvent& event);
	void OnListboxMouseDown(wxMouseEvent& event);

private:
	wxListBox* m_listbox;
	Signal<void()> m_itemDeselectionSignal;
	Signal<void(int)> m_itemSelectionSignal;
	Signal<void(int)> m_itemDoubleSelectionSignal;
};
