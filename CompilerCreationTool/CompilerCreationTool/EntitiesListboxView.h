#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/listbox.h>

class EntitiesListboxView : public wxPanel
{
public:
	explicit EntitiesListboxView(wxWindow* parent);

	void SetItems(const wxArrayString& items);
	void DeselectAll();

	int GetSelection() const;

	SignalScopedConnection DoOnItemDoubleSelection(Signal<void(int)>::slot_type);
	SignalScopedConnection DoOnItemSelection(Signal<void(int)>::slot_type);
	SignalScopedConnection DoOnFocusSet(Signal<void()>::slot_type);

private:
	void OnListboxItemDoubleSelection(wxCommandEvent& event);
	void OnListboxItemSelection(wxCommandEvent& event);

	void OnListboxMouseDown(wxMouseEvent& event);
	void OnFocusSet(wxFocusEvent& event);

private:
	wxListBox* m_listbox;
	Signal<void()> m_focusSignal;
	Signal<void(int)> m_itemSelectionSignal;
	Signal<void(int)> m_itemDoubleSelectionSignal;
};
