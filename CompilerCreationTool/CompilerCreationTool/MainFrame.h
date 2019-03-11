#pragma once
#include "Signal.h"
#include "MainPanel.h"
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/string.h>

class MainFrame : public wxFrame
{
public:
	using LanguageBuildSignal = Signal<void()>;
	using ParserRunSignal = Signal<void()>;
	using InfoQuerySignal = Signal<void()>;

public:
	MainFrame(const wxString& title, const wxSize& size);
	MainPanel* GetMainPanel();

	SignalScopedConnection DoOnLanguageBuildButtonPress(LanguageBuildSignal::slot_type slot);
	SignalScopedConnection DoOnParserRunButtonPress(ParserRunSignal::slot_type slot);
	SignalScopedConnection DoOnInfoQuery(InfoQuerySignal::slot_type slot);

private:
	void ShowAboutMessageBox();

	wxDECLARE_EVENT_TABLE();

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnBuild(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnInfo(wxCommandEvent& event);

	void OnHelp(wxCommandEvent& event);

private:
	MainPanel* m_panel;
	wxToolBar* mToolbar;
	LanguageBuildSignal m_languageBuildButtonPressSignal;
	ParserRunSignal m_parserRunButtonPressSignal;
	InfoQuerySignal m_infoQuerySignal;
};
