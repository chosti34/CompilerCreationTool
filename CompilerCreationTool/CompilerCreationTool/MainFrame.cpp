#include "pch.h"
#include "MainFrame.h"
#include <wx/artprov.h>
#include <wx/aui/auibar.h>
#include <wx/aui/dockart.h>

using namespace std::literals::string_literals;

namespace
{
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

	toolbar.AddTool(Buttons::New, wxT("New"), newIcon, wxT("New Document"));
	toolbar.AddTool(Buttons::Open, wxT("Open"), openIcon, wxT("Open Document"));
	toolbar.AddTool(Buttons::Save, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar.AddTool(Buttons::SaveAs, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Undo, wxT("Undo"), undoIcon, wxT("Undo command"));
	toolbar.AddTool(Buttons::Redo, wxT("Redo"), redoIcon, wxT("Redo command"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar.AddTool(Buttons::Run, wxT("Run"), runIcon, wxT("Run"));
	toolbar.AddTool(Buttons::Info, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Up, wxT("Up"), upIcon, wxT("Move Item Up"));
	toolbar.AddTool(Buttons::Down, wxT("Down"), downIcon, wxT("Move Item Down"));
	toolbar.AddTool(Buttons::Edit, wxT("Edit"), editIcon, wxT("Edit Item"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Help, wxT("Help"), helpIcon, wxT("Help"));
}
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
	m_auiManager.SetManagedWindow(this);
	m_auiManager.SetFlags(m_auiManager.GetFlags() | wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_ALLOW_ACTIVE_PANE);

	wxAuiDockArt* art = m_auiManager.GetArtProvider();
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

	m_declarationView = new GrammarView(m_notebook);
	m_editorView = new EditorView(m_notebook);
	m_statesView = new StatesView(m_notebook);
	m_treeView = new TreeView(m_notebook);

	m_notebook->AddPage(m_declarationView, "Grammar");
	m_notebook->AddPage(m_statesView, "States");
	m_notebook->AddPage(m_editorView, "Editor");
	m_notebook->AddPage(m_treeView, "AST");

	m_terminalsView = new EntitiesListboxView(this);
	m_actionsView = new EntitiesListboxView(this);
	m_outputView = new OutputView(this);

	m_auiManager.AddPane(m_notebook, wxAuiPaneInfo().Name("NotebookContent").CenterPane().PaneBorder(false));
	m_auiManager.AddPane(m_terminalsView, wxAuiPaneInfo().Name("TerminalsListbox").Caption("  Terminals").Left().Position(0).CloseButton(false).Dockable(true));
	m_auiManager.AddPane(m_actionsView, wxAuiPaneInfo().Name("ActionsListbox").Caption("  Actions").Left().Position(1).CloseButton(false).Dockable(true));
	m_auiManager.AddPane(m_outputView, wxAuiPaneInfo().Name("OutputPane").Caption("  Output").Bottom().Position(1).MaximizeButton(true).CloseButton(false).Dockable(true));

	SetMenuBar(CreateMenuBar());

	m_toolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*m_toolbar);
	m_toolbar->Realize();

	m_toolbar->EnableTool(Buttons::Run, false);
	m_toolbar->EnableTool(Buttons::Info, false);

	m_statusbar = CreateStatusBar(5);
	CustomizeStatusBar(*m_statusbar);

	Centre();
	m_auiManager.Update();
}

MainFrame::~MainFrame()
{
	m_auiManager.UnInit();
}

GrammarView* MainFrame::GetDeclarationView()
{
	return m_declarationView;
}

StatesView* MainFrame::GetStatesView()
{
	return m_statesView;
}

EditorView* MainFrame::GetEditorView()
{
	return m_editorView;
}

TreeView* MainFrame::GetTreeView()
{
	return m_treeView;
}

EntitiesListboxView* MainFrame::GetTerminalsView()
{
	return m_terminalsView;
}

EntitiesListboxView* MainFrame::GetActionsView()
{
	return m_actionsView;
}

OutputView* MainFrame::GetOutputView()
{
	return m_outputView;
}

wxStatusBar* MainFrame::GetStatusBar()
{
	return m_statusbar;
}

wxToolBar* MainFrame::GetToolBar()
{
	return m_toolbar;
}

SignalScopedConnection MainFrame::DoOnBuildButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Build].connect(slot);
}

SignalScopedConnection MainFrame::DoOnRunButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Run].connect(slot);
}

SignalScopedConnection MainFrame::DoOnInfoButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Info].connect(slot);
}

SignalScopedConnection MainFrame::DoOnUpButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Up].connect(slot);
}

SignalScopedConnection MainFrame::DoOnDownButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Down].connect(slot);
}

SignalScopedConnection MainFrame::DoOnEditButtonPress(ButtonPressSignal::slot_type slot)
{
	return m_signals[Buttons::Edit].connect(slot);
}

void MainFrame::InvokeSignal(Buttons::ID id)
{
	auto found = m_signals.find(id);
	assert(found != m_signals.end());
	found->second();
}

void MainFrame::OpenAboutDialog()
{
	wxMessageBox(
		"Tool for building compiler.",
		"About CompilerCreationTool",
		wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExit(wxCommandEvent&)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent&)
{
	OpenAboutDialog();
}

void MainFrame::OnBuild(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Build);
		m_toolbar->EnableTool(Buttons::Run, true);
		m_toolbar->EnableTool(Buttons::Info, true);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't build language: "s + ex.what() + "."s, "Can't Build", wxICON_WARNING);
	}
}

void MainFrame::OnRun(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Run);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Parse", wxICON_ERROR);
	}
}

void MainFrame::OnInfo(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Info);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Get Info", wxICON_ERROR);
	}
}

void MainFrame::OnHelp(wxCommandEvent&)
{
	OpenAboutDialog();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	std::cout << event.GetSize().x << " " << event.GetSize().y << std::endl;
	event.Skip();
}

void MainFrame::OnItemUp(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Up);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Move Item Up", wxICON_ERROR);
	}
}

void MainFrame::OnItemDown(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Down);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Move Item Down", wxICON_ERROR);
	}
}

void MainFrame::OnItemEdit(wxCommandEvent&)
{
	try
	{
		InvokeSignal(Buttons::Edit);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Edit Item", wxICON_ERROR);
	}
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TOOL(Buttons::Build, MainFrame::OnBuild)
	EVT_TOOL(Buttons::Run, MainFrame::OnRun)
	EVT_TOOL(Buttons::Info, MainFrame::OnInfo)
	EVT_TOOL(Buttons::Up, MainFrame::OnItemUp)
	EVT_TOOL(Buttons::Down, MainFrame::OnItemDown)
	EVT_TOOL(Buttons::Edit, MainFrame::OnItemEdit)
	EVT_TOOL(Buttons::Help, MainFrame::OnHelp)
wxEND_EVENT_TABLE()
