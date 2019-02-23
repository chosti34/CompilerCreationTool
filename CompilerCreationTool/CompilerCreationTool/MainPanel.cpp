#include "pch.h"
#include "MainPanel.h"

MainPanel::MainPanel(wxFrame* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_declarationView = new GrammarDeclarationView(m_notebook);
	m_statesView = new ParserStatesView(m_notebook);
	m_editorView = new CodeEditorView(m_notebook);

	m_notebook->AddPage(m_declarationView, wxT("Declaration"));
	m_notebook->AddPage(m_statesView, wxT("States"));
	m_notebook->AddPage(m_editorView, wxT("Editor"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}

GrammarDeclarationView* MainPanel::GetGrammarDeclarationView()
{
	return m_declarationView;
}

ParserStatesView* MainPanel::GetParsesStatesView()
{
	return m_statesView;
}

CodeEditorView* MainPanel::GetCodeEditorView()
{
	return m_editorView;
}
