#include "pch.h"
#include "Language.h"
#include "MainFrame.h"
#include "LanguageController.h"

namespace
{
const wxString TITLE = wxT("CompilerCreationTool");
const wxSize WINDOW_INITIAL_SIZE = { 720, 580 };
const wxSize WINDOW_MIN_SIZE = { 480, 320 };
}

class App : public wxApp
{
public:
	bool OnInit() override
	{
		m_frame = new MainFrame(TITLE, WINDOW_INITIAL_SIZE);
		m_frame->SetMinSize(WINDOW_MIN_SIZE);
		m_frame->Show(true);

		m_language = std::make_unique<Language>();
		m_controller = std::make_unique<LanguageController>(m_language.get(), m_frame);
		return true;
	}

private:
	MainFrame* m_frame;
	std::unique_ptr<Language> m_language;
	std::unique_ptr<LanguageController> m_controller;
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
