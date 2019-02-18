#include "pch.h"
#include "MainFrame.h"

#include "../Parser/ParserTable.h"

namespace
{
const wxString TITLE = wxT("CompilerCreationTool");
const wxSize WINDOW_INITIAL_SIZE = { 1024, 720 };
const wxSize WINDOW_MIN_SIZE = { 320, 320 };
}

class App : public wxApp
{
public:
	bool OnInit() override
	{
		MainFrame* frame = new MainFrame(TITLE, WINDOW_INITIAL_SIZE);
		frame->SetMinSize(wxSize(320, 320));
		frame->Show(true);
		return true;
	}
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
