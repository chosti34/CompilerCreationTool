#include "pch.h"
#include "Language.h"
#include "MainFrame.h"
#include "LanguageController.h"
#include "../Utils/command_utils.h"

namespace
{
const wxSize gcWindowInitialSize = { 945, 735 };
const wxSize gcWindowMinSize = { 480, 320 };
}

class App : public wxApp
{
public:
	bool OnInit() override
	{
		wxImage::AddHandler(new wxPNGHandler);

		m_language = std::make_unique<Language>(); // model
		m_frame = new MainFrame(wxEmptyString, gcWindowInitialSize); // view
		m_controller = std::make_unique<LanguageController>(m_language.get(), m_frame); // controller

		m_frame->SetMinSize(gcWindowMinSize);
		m_frame->Show(true);

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
