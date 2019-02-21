#include "pch.h"
#include "MainFrame.h"

namespace
{
const wxString TITLE = wxT("CompilerCreationTool");
const wxSize WINDOW_INITIAL_SIZE = { 640, 480 };
const wxSize WINDOW_MIN_SIZE = { 480, 320 };
}

class App : public wxApp
{
public:
	bool OnInit() override
	{
		MainFrame* frame = new MainFrame(TITLE, WINDOW_INITIAL_SIZE);
		frame->SetMinSize(WINDOW_MIN_SIZE);
		frame->Show(true);
		return true;
	}
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
