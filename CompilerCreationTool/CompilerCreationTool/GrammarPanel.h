#pragma once
#include "fwd.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <string>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);
	std::string GetGrammarText() const;
	void Split();

private:
	wxStyledTextCtrl* mTextControl;
	wxSplitterWindow* mSplitter;
	wxPanel* mLeftTextControlPanel;
	wxPanel* mRightNotebookPanel;
};
