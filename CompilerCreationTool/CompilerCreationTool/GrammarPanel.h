#pragma once
#include "fwd.h"
#include <wx/panel.h>

#include <wx/stc/stc.h>
#include <wx/richtext/richtextctrl.h>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);
	std::string GetGrammarText() const;

private:
	wxStyledTextCtrl* m_textCtrl;
};
