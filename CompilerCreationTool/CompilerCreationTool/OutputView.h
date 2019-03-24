#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>

class OutputView : public wxPanel
{
public:
	explicit OutputView(wxWindow* window);
	wxTextCtrl* GetTextCtrl();

private:
	wxTextCtrl* m_textCtrl;
};
