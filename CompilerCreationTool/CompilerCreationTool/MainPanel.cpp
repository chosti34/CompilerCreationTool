#include "pch.h"
#include "MainPanel.h"

MainPanel::MainPanel(wxFrame* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_declarationView = new DeclarationView(m_notebook);
	m_statesView = new StatesView(m_notebook);
	m_editorView = new EditorView(m_notebook);

	m_notebook->AddPage(m_declarationView, wxT("Declaration"));
	m_notebook->AddPage(m_statesView, wxT("States"));
	m_notebook->AddPage(m_editorView, wxT("Editor"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}

DeclarationView* MainPanel::GetGrammarDeclarationView()
{
	return m_declarationView;
}

StatesView* MainPanel::GetParsesStatesView()
{
	return m_statesView;
}

EditorView* MainPanel::GetCodeEditorView()
{
	return m_editorView;
}
