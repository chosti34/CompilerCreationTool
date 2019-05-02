#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/listctrl.h>

class EntitiesListboxView : public wxPanel
{
public:
	explicit EntitiesListboxView(
		wxWindow* parent,
		const std::string& leftColumn,
		const std::string& rightColumn
	);

	void SetItems(const std::vector<std::pair<std::string, std::string>> &items);
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
	void AdjustColumnWidth(int width);

private:
	void OnResize(wxSizeEvent& event);
	void OnListCtrlMouseDown(wxMouseEvent& event);
	void OnListCtrlItemSelection(wxListEvent& event);
	void OnListCtrlItemDoubleSelection(wxListEvent& event);

private:
	wxListCtrl* mListCtrl;
	std::string mLeftColumn;
	std::string mRightColumn;
	Signal<void()> m_itemDeselectionSignal;
	Signal<void(int)> m_itemSelectionSignal;
	Signal<void(int)> m_itemDoubleSelectionSignal;
};
