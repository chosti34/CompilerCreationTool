#pragma once
#include "fwd.h"
#include "MainPanel.h"

class Parser;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxSize& size);

private:
	wxDECLARE_EVENT_TABLE();

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnBuild(wxCommandEvent& event);

private:
	MainPanel* m_panel;
	std::unique_ptr<Parser> m_parser;
};
