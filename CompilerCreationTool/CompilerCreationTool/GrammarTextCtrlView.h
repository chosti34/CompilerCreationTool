#pragma once
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/textctrl.h>

class GrammarTextCtrlView : public wxPanel
{
public:
	GrammarTextCtrlView(wxWindow* parent);

private:
	wxTextCtrl* m_textCtrl;
};
