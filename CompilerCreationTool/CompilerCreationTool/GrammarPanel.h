#pragma once
#include "fwd.h"
#include <wx/panel.h>

class GrammarPanel : public wxPanel
{
public:
	GrammarPanel(wxWindow* parent);

private:
	wxTextCtrl* m_textCtrl;
};
