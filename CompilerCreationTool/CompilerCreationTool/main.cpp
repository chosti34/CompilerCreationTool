// CompilerCreationTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

class MyFrame : public wxFrame
{
public:
	enum Button
	{
		Hello
	};

	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
		: wxFrame(nullptr, wxID_ANY, title, pos, size)
	{
		wxMenu* fileItem = new wxMenu;
		fileItem->Append(Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
		fileItem->AppendSeparator();
		fileItem->Append(wxID_EXIT);

		wxMenu* helpItem = new wxMenu;
		helpItem->Append(wxID_ABOUT);

		wxMenuBar* menuBar = new wxMenuBar;
		menuBar->Append(fileItem, "&File");
		menuBar->Append(helpItem, "&Help");
		SetMenuBar(menuBar);

		CreateStatusBar();
		SetStatusText("Welcome to wxWidgets!");
	}

private:
	void OnHello(wxCommandEvent& event)
	{
		wxMessageBox("Hello world from wxWidgets!");
	}

	void OnExit(wxCommandEvent& event)
	{
		Close(true);
	}

	void OnAbout(wxCommandEvent& event)
	{
		wxMessageBox(
			"This is a wxWidgets' Hello world sample",
			"About Hello World",
			wxOK | wxICON_INFORMATION);
	}

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(MyFrame::Hello, MyFrame::OnHello)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

class App : public wxApp
{
public:
	bool OnInit() override
	{
		MyFrame* frame = new MyFrame("CompilerCreationTool", wxPoint(50, 50), wxSize(450, 340));
		frame->Show(true);
		return true;
	}
};

#ifndef _DEBUG
	wxIMPLEMENT_APP(App);
#else
	wxIMPLEMENT_APP_CONSOLE(App);
#endif
