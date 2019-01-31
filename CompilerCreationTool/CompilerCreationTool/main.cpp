// CompilerCreationTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "MainFrame.h"

class App : public wxApp
{
public:
	bool OnInit() override
	{
		MainFrame* frame = new MainFrame("CompilerCreationTool", wxSize(640, 480));
		frame->Show(true);
		return true;
	}
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
