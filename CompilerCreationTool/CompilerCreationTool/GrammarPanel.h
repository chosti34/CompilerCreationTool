#pragma once
#include "fwd.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);
	std::string GetGrammarText() const;

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_textCtrl;
};
