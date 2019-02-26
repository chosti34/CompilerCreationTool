#include "pch.h"
#include "MainFrame.h"
#include <wx/statline.h>
#include <wx/artprov.h>

namespace
{
enum ButtonID
{
	New,
	Open,
	Save,
	SaveAs,
	Undo,
	Redo,
	Build,
	Run,
	Info,
	Help
};

wxMenuBar* CreateMenuBar()
{
	wxMenuBar* menubar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT);

	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT);

	menubar->Append(file, wxT("&File"));
	menubar->Append(help, wxT("&Help"));
	return menubar;
}

void AddTools(wxToolBar& toolbar)
{
	const wxSize iconSize(24, 24);

	wxIcon newIcon = wxArtProvider::GetIcon(wxART_NEW, wxART_OTHER, iconSize);
	wxIcon saveIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE, wxART_OTHER, iconSize);
	wxIcon saveAsIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE_AS, wxART_OTHER, iconSize);
	wxIcon infoIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, iconSize);
	wxIcon buildIcon = wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_OTHER, iconSize);
	wxIcon openIcon = wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, iconSize);
	wxIcon undoIcon = wxArtProvider::GetIcon(wxART_UNDO, wxART_OTHER, iconSize);
	wxIcon redoIcon = wxArtProvider::GetIcon(wxART_REDO, wxART_OTHER, iconSize);
	wxIcon helpIcon = wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, iconSize);
	wxIcon fileIcon = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_OTHER, iconSize);

	toolbar.AddTool(ButtonID::New, wxT("New"), newIcon, wxT("New Document"));
	toolbar.AddTool(ButtonID::Open, wxT("Open"), openIcon, wxT("Open Document"));
	toolbar.AddTool(ButtonID::Save, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar.AddTool(ButtonID::SaveAs, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Undo, wxT("Undo"), undoIcon, wxT("Undo Command"));
	toolbar.AddTool(ButtonID::Redo, wxT("Redo"), redoIcon, wxT("Redo Command"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar.AddTool(ButtonID::Run, wxT("Run"), fileIcon, wxT("Run"));
	toolbar.AddTool(ButtonID::Info, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar.AddTool(ButtonID::Help, wxT("Help"), helpIcon, wxT("Help"));
}
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
	m_panel = new MainPanel(this);
	SetMenuBar(CreateMenuBar());

	wxToolBar* toolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*toolbar);
	toolbar->Realize();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticLine* line = new wxStaticLine(this, wxID_ANY);
	sizer->Add(line, 0, wxEXPAND);
	sizer->Add(m_panel, 1, wxEXPAND);

	SetSizer(sizer);
	CreateStatusBar();
	SetStatusText(wxT("Welcome to CompilerCreationTool!"));
	Centre();

	m_panel->GetParsesStatesView()->AdjustColumnWidth();
	m_panel->GetCodeEditorView()->SplitPanels(0.5f);
	m_panel->GetGrammarDeclarationView()->SplitPanels(0.6f);
}

MainPanel* MainFrame::GetMainPanel()
{
	return m_panel;
}

SignalScopedConnection MainFrame::DoOnLanguageBuildButtonPress(
	LanguageBuildSignal::slot_type slot)
{
	return m_languageBuildButtonPressSignal.connect(slot);
}

SignalScopedConnection MainFrame::DoOnParserRunButtonPress(
	ParserRunSignal::slot_type slot)
{
	return m_parserRunButtonPressSignal.connect(slot);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(
		"Tool for building compiler.",
		"About CompilerCreationTool",
		wxOK | wxICON_INFORMATION
	);
}

void MainFrame::OnBuild(wxCommandEvent& event)
{
	try
	{
		m_languageBuildButtonPressSignal();
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what());
	}
}

void MainFrame::OnRun(wxCommandEvent& event)
{
	try
	{
		m_parserRunButtonPressSignal();
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what());
	}
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TOOL(ButtonID::Build, MainFrame::OnBuild)
	EVT_TOOL(ButtonID::Run, MainFrame::OnRun)
wxEND_EVENT_TABLE()
