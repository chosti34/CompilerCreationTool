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

wxStatusBar* CreateStatusBar(wxFrame& frame)
{
	wxStatusBar* statusbar = frame.CreateStatusBar(StatusBarFields::Count);

	statusbar->SetStatusText(wxT("Welcome to CompilerCreationTool!"), StatusBarFields::HelpInfo);
	statusbar->SetStatusText(wxT(""), StatusBarFields::Line);
	statusbar->SetStatusText(wxT(""), StatusBarFields::Column);
	statusbar->SetStatusText(wxT(""), StatusBarFields::Ch);
	statusbar->SetStatusText(wxT(""), StatusBarFields::ContextInfo);

	const int styles[] = { wxSB_FLAT, wxSB_FLAT, wxSB_FLAT, wxSB_FLAT, wxSB_FLAT };
	statusbar->SetStatusStyles(5, styles);

	const int widths[] = { -5, -1, -1, -2, -1 };
	statusbar->SetStatusWidths(5, widths);

	return statusbar;
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
	// Menu bar
	SetMenuBar(CreateMenuBar());

	// Tool bar
	mToolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*mToolbar);
	mToolbar->Realize();

	mToolbar->EnableTool(Buttons::Run, false);
	mToolbar->EnableTool(Buttons::Info, false);
	mToolbar->EnableTool(Buttons::Up, false);
	mToolbar->EnableTool(Buttons::Down, false);
	mToolbar->EnableTool(Buttons::Edit, false);

	// AUI panels
	mAuiManager.SetManagedWindow(this);
	mAuiManager.SetFlags(mAuiManager.GetFlags() | wxAUI_MGR_LIVE_RESIZE |
		wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_ALLOW_ACTIVE_PANE);

	wxAuiDockArt* art = mAuiManager.GetArtProvider();
	art->SetMetric(wxAUI_DOCKART_SASH_SIZE, 2);
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, 22);
	art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, 0);
	// This dock art's metric causes too much lags
	// art->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_HORIZONTAL);
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, wxColour(57, 193, 239));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(0, 191, 255));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, wxColour(255, 255, 255));
	art->SetFont(wxAUI_DOCKART_CAPTION_FONT, wxFont(wxFontInfo().Family(wxFONTFAMILY_SWISS)));

	mNotebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_SPLIT |
		wxAUI_NB_WINDOWLIST_BUTTON | wxNO_BORDER);
	wxAuiManager& notebookAuiManager = const_cast<wxAuiManager&>(mNotebook->GetAuiManager());
	notebookAuiManager.SetFlags(notebookAuiManager.GetFlags() | wxAUI_MGR_LIVE_RESIZE);
	mNotebook->SetArtProvider(new wxAuiSimpleTabArt);

	mGrammarView = new TextView(mNotebook);
	mEditorView = new TextView(mNotebook);
	mStatesView = new StatesView(mNotebook);
	mTreeView = new TreeView(mNotebook);

	mNotebook->AddPage(mGrammarView, "Grammar");
	mNotebook->AddPage(mStatesView, "States");
	mNotebook->AddPage(mEditorView, "Editor");
	mNotebook->AddPage(mTreeView, "AST");

	mTerminalsView = new EntitiesListboxView(this);
	mActionsView = new EntitiesListboxView(this);
	mOutputView = new OutputView(this);

	mAuiManager.AddPane(mNotebook, wxAuiPaneInfo().
		Name("NotebookContent").CenterPane().PaneBorder(false));
	mAuiManager.AddPane(mTerminalsView, wxAuiPaneInfo().Name("TerminalsListbox").
		Caption("  Terminals").Left().Position(0).CloseButton(false).Dockable(true));
	mAuiManager.AddPane(mActionsView, wxAuiPaneInfo().Name("ActionsListbox").
		Caption("  Actions").Left().Position(1).CloseButton(false).Dockable(true));
	mAuiManager.AddPane(mOutputView, wxAuiPaneInfo().Name("OutputPane").
		Caption("  Output").Bottom().Position(1).MaximizeButton(true).CloseButton(false).Dockable(true));
	mAuiManager.Update();

	// Status bar
	mStatusbar = ::CreateStatusBar(*this);
	Centre();
}

MainFrame::~MainFrame()
{
	mAuiManager.UnInit();
}

TextView* MainFrame::GetDeclarationView()
{
	return mGrammarView;
}

StatesView* MainFrame::GetStatesView()
{
	return mStatesView;
}

TextView* MainFrame::GetEditorView()
{
	return mEditorView;
}

TreeView* MainFrame::GetTreeView()
{
	return mTreeView;
}

EntitiesListboxView* MainFrame::GetTerminalsView()
{
	return mTerminalsView;
}

EntitiesListboxView* MainFrame::GetActionsView()
{
	return mActionsView;
}

OutputView* MainFrame::GetOutputView()
{
	return mOutputView;
}

wxStatusBar* MainFrame::GetStatusBar()
{
	return mStatusbar;
}

wxToolBar* MainFrame::GetToolBar()
{
	return mToolbar;
}

SignalScopedConnection MainFrame::DoOnButtonPress(Buttons::ID button,
	ButtonPressSignal::slot_type slot)
{
	return mSignals[button].connect(slot);
}

void MainFrame::SendButtonPressedSignal(Buttons::ID buttonID)
{
	auto found = mSignals.find(buttonID);
	assert(found != mSignals.end());
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
		SendButtonPressedSignal(Buttons::Build);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't build language: "s + ex.what() + "."s, "Can't Build", wxICON_ERROR);
	}
}

void MainFrame::OnRun(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::Run);
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
		SendButtonPressedSignal(Buttons::Info);
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
	event.Skip(true);
}

void MainFrame::OnNew(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::New);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Create New Project", wxICON_ERROR);
	}
}

void MainFrame::OnOpen(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::Open);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Open Project", wxICON_ERROR);
	}
}

void MainFrame::OnSave(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::Save);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Save Project", wxICON_ERROR);
	}
}

void MainFrame::OnSaveAs(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::SaveAs);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), "Can't Create New Language", wxICON_ERROR);
	}
}

void MainFrame::OnItemUp(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::Up);
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
		SendButtonPressedSignal(Buttons::Down);
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
		SendButtonPressedSignal(Buttons::Edit);
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
	EVT_TOOL(Buttons::New, MainFrame::OnNew)
	EVT_TOOL(Buttons::Open, MainFrame::OnOpen)
	EVT_TOOL(Buttons::Save, MainFrame::OnSave)
	EVT_TOOL(Buttons::SaveAs, MainFrame::OnSaveAs)
	EVT_TOOL(Buttons::Build, MainFrame::OnBuild)
	EVT_TOOL(Buttons::Run, MainFrame::OnRun)
	EVT_TOOL(Buttons::Info, MainFrame::OnInfo)
	EVT_TOOL(Buttons::Up, MainFrame::OnItemUp)
	EVT_TOOL(Buttons::Down, MainFrame::OnItemDown)
	EVT_TOOL(Buttons::Edit, MainFrame::OnItemEdit)
	EVT_TOOL(Buttons::Help, MainFrame::OnHelp)
wxEND_EVENT_TABLE()
