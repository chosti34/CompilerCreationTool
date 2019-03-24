#pragma once
#include "../Parser/IParserTable.h"
#include <wx/listctrl.h>
#include <wx/panel.h>

class StatesView : public wxPanel
{
public:
	explicit StatesView(wxWindow* parent);

	void SetParserTable(const IParserTable& table);
	void AdjustColumnWidth();

private:
	void OnListResize(wxSizeEvent& event);

private:
	wxListView* m_list;
};
