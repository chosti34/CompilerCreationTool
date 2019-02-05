#pragma once
#include "fwd.h"
#include <wx/panel.h>

class MainPanel : public wxPanel
{
public:
	MainPanel(wxFrame* parent);

private:
	wxNotebook* m_notebook;
	GrammarPanel* m_grammarPanel;
	TablePanel* m_tablePanel;
	EnvironmentPanel* m_environmentPanel;
};
