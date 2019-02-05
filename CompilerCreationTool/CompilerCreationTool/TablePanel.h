#pragma once
#include "fwd.h"
#include <wx/panel.h>

class TablePanel : public wxPanel
{
public:
	TablePanel(wxWindow* parent);

private:
	wxListView* m_table;
};
