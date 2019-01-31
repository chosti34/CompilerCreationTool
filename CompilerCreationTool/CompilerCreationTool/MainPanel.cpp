#include "pch.h"
#include "MainPanel.h"

MainPanel::MainPanel(wxFrame* parent)
	: wxPanel(parent, wxID_ANY)
	, m_notebook(new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
	, m_grammarTextCtrlView(new GrammarTextCtrlView(m_notebook))
	, m_parserTableView(new ParserTableView(m_notebook))
	, m_environmentView(new ParserEnvironmentView(m_notebook))
{
	m_notebook->AddPage(m_grammarTextCtrlView, wxT("Grammar"));
	m_notebook->AddPage(m_parserTableView, wxT("Table"));
	m_notebook->AddPage(m_environmentView, wxT("Environment"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}
