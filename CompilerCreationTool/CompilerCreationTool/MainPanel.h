#pragma once
#include "GrammarTextCtrlView.h"
#include "ParserTableView.h"
#include "ParserEnvironmentView.h"
#include <wx/frame.h>
#include <wx/notebook.h>

class MainPanel : public wxPanel
{
public:
	MainPanel(wxFrame* parent);

private:
	wxNotebook* m_notebook;
	GrammarTextCtrlView* m_grammarTextCtrlView;
	ParserTableView* m_parserTableView;
	ParserEnvironmentView* m_environmentView;
};
