#pragma once
#include "MainPanel.h"
#include <wx/string.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxSize& size);

private:
	wxDECLARE_EVENT_TABLE();
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

private:
	MainPanel* m_panel;
};
