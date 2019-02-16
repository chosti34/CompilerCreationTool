#pragma once
#include "fwd.h"
#include <wx/panel.h>

class TablePanel : public wxPanel
{
public:
	TablePanel(wxWindow* parent);

	wxListView* GetListView();
	const wxListView* GetListView() const;

	void AdjustColumnWidths();

private:
	wxListView* m_list;
};
