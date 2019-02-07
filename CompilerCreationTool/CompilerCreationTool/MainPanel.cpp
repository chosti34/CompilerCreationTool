#include "pch.h"
#include "MainPanel.h"
#include "TablePanel.h"
#include "GrammarPanel.h"
#include "EnvironmentPanel.h"

#include <wx/frame.h>
#include <wx/notebook.h>

MainPanel::MainPanel(wxFrame* parent)
	: wxPanel(parent, wxID_ANY)
	, m_notebook(new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
	, m_grammarPanel(new GrammarPanel(m_notebook))
	, m_tablePanel(new TablePanel(m_notebook))
	, m_environmentPanel(new EnvironmentPanel(m_notebook))
{
	m_notebook->AddPage(m_grammarPanel, wxT("Grammar"));
	m_notebook->AddPage(m_tablePanel, wxT("Table"));
	m_notebook->AddPage(m_environmentPanel, wxT("Environment"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}

GrammarPanel* MainPanel::GetGrammarPanel()
{
	return m_grammarPanel;
}

TablePanel* MainPanel::GetTablePanel()
{
	return m_tablePanel;
}

EnvironmentPanel* MainPanel::GetEnvironmentPanel()
{
	return m_environmentPanel;
}
