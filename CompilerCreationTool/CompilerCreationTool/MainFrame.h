#pragma once
#include "fwd.h"
#include "MainPanel.h"
#include "../Parser/IParser.h"

#include "Compiler.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxSize& size);

private:
	wxDECLARE_EVENT_TABLE();

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnBuild(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);

private:
	MainPanel* m_panel;
	std::unique_ptr<Compiler> m_compiler;
};
