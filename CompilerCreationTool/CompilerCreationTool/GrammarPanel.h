#pragma once
#include "fwd.h"
#include <wx/panel.h>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);

	std::string GetGrammarText() const;

private:
	wxTextCtrl* m_textCtrl;
};
