#include "pch.h"
#include "MainFrame.h"

#include "../Parser/ParserTable.h"

namespace
{
const wxString TITLE = wxT("CompilerCreationTool");
const wxSize WINDOW_SIZE = { 1024, 720 };
}

class App : public wxApp
{
public:
	bool OnInit() override
	{
		MainFrame* frame = new MainFrame(TITLE, WINDOW_SIZE);
		frame->Show(true);
		return true;
	}
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
