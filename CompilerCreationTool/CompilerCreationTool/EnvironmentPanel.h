#pragma once
#include "fwd.h"
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

class EnvironmentPanel : public wxPanel
{
public:
	EnvironmentPanel(wxWindow* parent);

	wxStyledTextCtrl* GetInputControl();
	wxStyledTextCtrl* GetOutputControl();

	void Split();

private:
	wxDECLARE_EVENT_TABLE();

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxStyledTextCtrl* m_output;
	wxPanel* m_left;
	wxPanel* m_right;
};
