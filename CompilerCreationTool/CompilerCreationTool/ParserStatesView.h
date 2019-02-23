#pragma once
#include "../Parser/IParserTable.h"
#include <wx/listctrl.h>
#include <wx/panel.h>

class ParserStatesView : public wxPanel
{
public:
	ParserStatesView(wxWindow* parent);

	void SetParserTable(const IParserTable& table);
	void AdjustColumnWidth();

private:
	wxListView* m_list;
};
