#include "pch.h"
#include "MainFrame.h"
#include <wx/artprov.h>
#include <wx/statline.h>

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
	, m_panel(new MainPanel(this))
{
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT);

	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT);

	wxMenuBar* menubar = new wxMenuBar;
	menubar->Append(file, wxT("&File"));
	menubar->Append(help, wxT("&Help"));
	SetMenuBar(menubar);

	wxToolBar* toolbar = CreateToolBar(wxTB_FLAT);

	wxIcon newIcon = wxArtProvider::GetIcon(wxART_NEW, wxART_OTHER, wxSize(24, 24));
	wxIcon saveIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE, wxART_OTHER, wxSize(24, 24));
	wxIcon saveAsIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE_AS, wxART_OTHER, wxSize(24, 24));
	wxIcon infoIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, wxSize(24, 24));
	wxIcon buildIcon = wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_OTHER, wxSize(24, 24));

	toolbar->AddTool(1, wxT("New"), newIcon, wxT("New Document"));
	toolbar->AddTool(2, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar->AddTool(3, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar->AddSeparator();
	toolbar->AddTool(4, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar->AddTool(5, wxT("Build"), buildIcon, wxT("Build"));

	toolbar->Realize();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticLine* line = new wxStaticLine(this, wxID_ANY);
	sizer->Add(line, 0, wxEXPAND);
	sizer->Add(m_panel, 1, wxEXPAND);
	SetSizer(sizer);

	CreateStatusBar();
	SetStatusText(wxT("Welcome to CompilerCreationTool!"));

	Centre();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(
		"This is a wxWidgets' Hello world sample",
		"About Hello World",
		wxOK | wxICON_INFORMATION
	);
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
