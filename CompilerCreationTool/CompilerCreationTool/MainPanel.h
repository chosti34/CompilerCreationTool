#pragma once
#include "CodeEditorView.h"
#include "ParserStatesView.h"
#include "GrammarDeclarationView.h"
#include <wx/notebook.h>

class MainPanel : public wxPanel
{
public:
	MainPanel(wxFrame* parent);

	CodeEditorView* GetCodeEditorView();
	ParserStatesView* GetParsesStatesView();
	GrammarDeclarationView* GetGrammarDeclarationView();

private:
	wxNotebook* m_notebook;
	CodeEditorView* m_editorView;
	ParserStatesView* m_statesView;
	GrammarDeclarationView* m_declarationView;
};
