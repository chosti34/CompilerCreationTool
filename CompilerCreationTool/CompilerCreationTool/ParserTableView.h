#pragma once
#include <wx/panel.h>
#include <wx/window.h>
#include <wx/listctrl.h>

class ParserTableView : public wxPanel
{
public:
	ParserTableView(wxWindow* parent);

private:
	wxListView* m_table;
};
