#include "pch.h"
#include "MainFrame.h"

#include <wx/artprov.h>
#include <wx/aui/auibar.h>
#include <wx/aui/dockart.h>

namespace
{
enum ButtonID
{
	New,
	Open,
	Save,
	SaveAs,
	//
	Undo,
	Redo,
	//
	Build,
	Run,
	Info,
	//
	Up,
	Down,
	Edit,
	//
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

void CustomizeStatusBar(wxStatusBar& statusbar)
{
	statusbar.SetStatusText(wxT("Welcome to CompilerCreationTool!"), 0);
	statusbar.SetStatusText(wxT("Ln 1"), 1);
	statusbar.SetStatusText(wxT("Col 1"), 2);
	statusbar.SetStatusText(wxT("Ch 1"), 3);
	statusbar.SetStatusText(wxT("Select Item..."), 4);

	const int styles[] = { wxSB_FLAT, wxSB_FLAT, wxSB_FLAT, wxSB_FLAT, wxSB_FLAT };
	statusbar.SetStatusStyles(5, styles);

	const int widths[] = { -5, -1, -1, -2, -1 };
	statusbar.SetStatusWidths(5, widths);
}

void AddTools(wxToolBar& toolbar)
{
	const wxSize iconSize(24, 24);

	wxIcon openIcon = wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, iconSize);
	wxIcon newIcon = wxArtProvider::GetIcon(wxART_NEW, wxART_OTHER, iconSize);
	wxIcon saveIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE, wxART_OTHER, iconSize);
	wxIcon saveAsIcon = wxArtProvider::GetIcon(wxART_FILE_SAVE_AS, wxART_OTHER, iconSize);

	wxIcon undoIcon = wxArtProvider::GetIcon(wxART_UNDO, wxART_OTHER, iconSize);
	wxIcon redoIcon = wxArtProvider::GetIcon(wxART_REDO, wxART_OTHER, iconSize);

	wxIcon buildIcon = wxArtProvider::GetIcon(wxART_PASTE, wxART_OTHER, iconSize);
	wxIcon runIcon = wxArtProvider::GetIcon(wxART_COPY, wxART_OTHER, iconSize);
	wxIcon infoIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, iconSize);

	wxIcon upIcon = wxArtProvider::GetIcon(wxART_GO_UP, wxART_OTHER, iconSize);
	wxIcon downIcon = wxArtProvider::GetIcon(wxART_GO_DOWN, wxART_OTHER, iconSize);
	wxIcon editIcon = wxArtProvider::GetIcon(wxART_FULL_SCREEN, wxART_OTHER, iconSize);

	wxIcon helpIcon = wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, iconSize);

	toolbar.AddTool(ButtonID::New, wxT("New"), newIcon, wxT("New Document"));
	toolbar.AddTool(ButtonID::Open, wxT("Open"), openIcon, wxT("Open Document"));
	toolbar.AddTool(ButtonID::Save, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar.AddTool(ButtonID::SaveAs, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Undo, wxT("Undo"), undoIcon, wxT("Undo command"));
	toolbar.AddTool(ButtonID::Redo, wxT("Redo"), redoIcon, wxT("Redo command"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar.AddTool(ButtonID::Run, wxT("Run"), runIcon, wxT("Run"));
	toolbar.AddTool(ButtonID::Info, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Up, wxT("Up"), upIcon, wxT("Move Item Up"));
	toolbar.AddTool(ButtonID::Down, wxT("Down"), downIcon, wxT("Move Item Down"));
	toolbar.AddTool(ButtonID::Edit, wxT("Edit"), editIcon, wxT("Edit Item"));
	toolbar.AddSeparator();

	toolbar.AddTool(ButtonID::Help, wxT("Help"), helpIcon, wxT("Help"));
}
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
	m_manager.SetManagedWindow(this);
	m_manager.SetFlags(m_manager.GetFlags() | wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_ALLOW_ACTIVE_PANE);

	wxAuiDockArt* art = m_manager.GetArtProvider();
	art->SetMetric(wxAUI_DOCKART_SASH_SIZE, 2);
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, 22);
	art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, 0);
	// This dock art's metric causes too much lags
	// art->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_HORIZONTAL);
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, wxColour(57, 193, 239));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(0, 191, 255));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, wxColour(255, 255, 255));
	art->SetFont(wxAUI_DOCKART_CAPTION_FONT, wxFont(wxFontInfo().Family(wxFONTFAMILY_SWISS)));

	m_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_WINDOWLIST_BUTTON | wxNO_BORDER);
	wxAuiManager& manager = const_cast<wxAuiManager&>(m_notebook->GetAuiManager());
	manager.SetFlags(manager.GetFlags() | wxAUI_MGR_LIVE_RESIZE);
	m_notebook->SetArtProvider(new wxAuiSimpleTabArt);

	m_notebook->AddPage(new DeclarationView(m_notebook), "Grammar");
	m_notebook->AddPage(new EditorView(m_notebook), "Editor");
	m_notebook->AddPage(new StatesView(m_notebook), "Parser");
	m_notebook->AddPage(new TreeView(m_notebook), "AST");

	m_manager.AddPane(m_notebook, wxAuiPaneInfo().Name("NotebookContent").CenterPane().PaneBorder(false));
	m_manager.AddPane(new StatesView(this), wxAuiPaneInfo().Name("TerminalsListbox").Caption("  Terminals").Left().Position(0).CloseButton(false).Dockable(true));
	m_manager.AddPane(new StatesView(this), wxAuiPaneInfo().Name("ActionsListbox").Caption("  Actions").Left().Position(1).CloseButton(false).Dockable(true));
	m_manager.AddPane(new TreeView(this), wxAuiPaneInfo().Name("OutputPane").Caption("  Output").Bottom().Position(1).MaximizeButton(true).CloseButton(false).Dockable(true));

	SetMenuBar(CreateMenuBar());

	mToolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*mToolbar);
	mToolbar->Realize();

	mToolbar->EnableTool(ButtonID::Run, false);
	mToolbar->EnableTool(ButtonID::Info, false);

	mStatusBar = CreateStatusBar(5);
	CustomizeStatusBar(*mStatusBar);

	Centre();
	m_manager.Update();
}

MainFrame::~MainFrame()
{
	m_manager.UnInit();
}

MainPanel* MainFrame::GetMainPanel()
{
	return m_panel;
}

wxStatusBar* MainFrame::GetStatusBar()
{
	return mStatusBar;
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

SignalScopedConnection MainFrame::DoOnInfoQuery(
	InfoQuerySignal::slot_type slot)
{
	return m_infoQuerySignal.connect(slot);
}

void MainFrame::ShowAboutMessageBox()
{
	wxMessageBox(
		"Tool for building compiler.",
		"About CompilerCreationTool",
		wxOK | wxICON_INFORMATION
	);
}

void MainFrame::OnExit(wxCommandEvent&)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent&)
{
	ShowAboutMessageBox();
}

void MainFrame::OnBuild(wxCommandEvent&)
{
	try
	{
		m_languageBuildButtonPressSignal();
		mToolbar->EnableTool(ButtonID::Run, true);
		mToolbar->EnableTool(ButtonID::Info, true);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Language Building Error", wxICON_WARNING);
	}
}

void MainFrame::OnRun(wxCommandEvent&)
{
	try
	{
		m_parserRunButtonPressSignal();
		mToolbar->EnableTool(ButtonID::Run, true);
		mToolbar->EnableTool(ButtonID::Info, true);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Parsing Error", wxICON_WARNING);
	}
}

void MainFrame::OnInfo(wxCommandEvent&)
{
	try
	{
		m_infoQuerySignal();
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what());
	}
}

void MainFrame::OnHelp(wxCommandEvent&)
{
	ShowAboutMessageBox();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	std::cout << event.GetSize().x << " " << event.GetSize().y << std::endl;
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TOOL(ButtonID::Build, MainFrame::OnBuild)
	EVT_TOOL(ButtonID::Run, MainFrame::OnRun)
	EVT_TOOL(ButtonID::Info, MainFrame::OnInfo)
	EVT_TOOL(ButtonID::Help, MainFrame::OnHelp)
wxEND_EVENT_TABLE()
