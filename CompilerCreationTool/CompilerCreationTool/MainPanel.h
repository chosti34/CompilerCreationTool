#pragma once
#include "EditorView.h"
#include "StatesView.h"
#include "DeclarationView.h"
#include "TreeView.h"
#include <wx/notebook.h>

class MainPanel : public wxPanel
{
public:
	MainPanel(wxFrame* parent);

	EditorView* GetCodeEditorView();
	StatesView* GetParsesStatesView();
	DeclarationView* GetGrammarDeclarationView();
	TreeView* GetTreeView();

private:
	wxNotebook* m_notebook;
	EditorView* m_editorView;
	StatesView* m_statesView;
	DeclarationView* m_declarationView;
	TreeView* mTreeView;
};
