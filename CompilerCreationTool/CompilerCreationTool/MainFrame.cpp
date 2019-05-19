#include "pch.h"
#include "MainFrame.h"
#include "XpmImages.h"
#include <wx/artprov.h>
#include <wx/aui/auibar.h>
#include <wx/aui/dockart.h>
#include <wx/aboutdlg.h>

using namespace std::literals::string_literals;

namespace
{
wxMenuBar* CreateMenuBar()
{
	wxMenuBar* menubar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(wxID_NEW);
	file->Append(wxID_OPEN);
	file->Append(wxID_SAVE);
	file->Append(wxID_SAVEAS);
	file->AppendSeparator();
	file->Append(wxID_EXIT);

	wxMenu* edit = new wxMenu;
	edit->Append(Buttons::Up, "Move item up");
	edit->Append(Buttons::Down, "Move item down");
	edit->Append(Buttons::Edit, "Item settings...");

	wxMenu* view = new wxMenu;
	view->AppendCheckItem(
		Buttons::LogMessages,
		"Log only action's messages",
		"Check this item if you want to log only action's messages");
	view->AppendCheckItem(
		Buttons::EnableCodegen,
		"Enable code generation",
		"Check this item if you want to generate LLVM code after parsing"
	);
	view->AppendSeparator();
	view->Append(Buttons::Clear, "Clear output", nullptr, "Clear output window's content");

	wxMenu* language = new wxMenu;
	language->Append(Buttons::Build, "Build");
	language->Append(Buttons::Run, "Run");
	language->Append(Buttons::Info, "Get info");

	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT);

	menubar->Append(file, wxT("&File"));
	menubar->Append(edit, wxT("&Edit"));
	menubar->Append(view, wxT("&View"));
	menubar->Append(language, wxT("&Language"));
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

	wxIcon buildIcon(gcBuildXpm);
	wxIcon runIcon(gcRunXpm);
	wxIcon infoIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, iconSize);

	wxIcon upIcon = wxArtProvider::GetIcon(wxART_GO_UP, wxART_OTHER, iconSize);
	wxIcon downIcon = wxArtProvider::GetIcon(wxART_GO_DOWN, wxART_OTHER, iconSize);
	wxIcon editIcon(gcSettingsXpm);

	wxIcon helpIcon = wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, iconSize);

	toolbar.AddTool(Buttons::New, wxT("New"), newIcon, wxT("New document"));
	toolbar.AddTool(Buttons::Open, wxT("Open"), openIcon, wxT("Open document"));
	toolbar.AddTool(Buttons::Save, wxT("Save"), saveIcon, wxT("Save document"));
	toolbar.AddTool(Buttons::SaveAs, wxT("Save As"), saveAsIcon, wxT("Save document as"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar.AddTool(Buttons::Run, wxT("Run"), runIcon, wxT("Run"));
	toolbar.AddTool(Buttons::Info, wxT("Info"), infoIcon, wxT("Get info"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Up, wxT("Up"), upIcon, wxT("Move item up"));
	toolbar.AddTool(Buttons::Down, wxT("Down"), downIcon, wxT("Move item down"));
	toolbar.AddTool(Buttons::Edit, wxT("Edit"), editIcon, wxT("Edit item"));
	toolbar.AddSeparator();

	toolbar.AddTool(Buttons::Help, wxT("Help"), helpIcon, wxT("Help"));
}
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
	SetIcon(wxIcon(gcAppIcon));

	// Menubar
	mMenubar = CreateMenuBar();
	SetMenuBar(mMenubar);

	// Toolbar
	mToolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*mToolbar);
	mToolbar->Realize();

	mMenubar->Enable(Buttons::LogMessages, false);
	mMenubar->Enable(Buttons::EnableCodegen, false);
	mMenubar->Enable(Buttons::Run, false);
	mMenubar->Enable(Buttons::Info, false);
	mMenubar->Enable(Buttons::Up, false);
	mMenubar->Enable(Buttons::Down, false);
	mMenubar->Enable(Buttons::Edit, false);
	mToolbar->EnableTool(Buttons::Run, false);
	mToolbar->EnableTool(Buttons::Info, false);
	mToolbar->EnableTool(Buttons::Up, false);
	mToolbar->EnableTool(Buttons::Down, false);
	mToolbar->EnableTool(Buttons::Edit, false);
	mToolbar->EnableTool(Buttons::Save, false);
	mToolbar->EnableTool(Buttons::SaveAs, false);

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

	// Active color is set to default
	// art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, wxColour(255, 255, 200));
	// art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(255, 242, 157));
	// art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, wxColour(0, 0, 0));
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

	mTerminalsView = new EntitiesListboxView(this, "Name", "Regex");
	mActionsView = new EntitiesListboxView(this, "Name", "Function");
	mOutputView = new OutputView(this);

	mAuiManager.AddPane(mNotebook, wxAuiPaneInfo().Name("NotebookContent").CenterPane().PaneBorder(false));
	mAuiManager.AddPane(mTerminalsView, wxAuiPaneInfo().Name("TerminalsListbox").Caption("  Terminals").Left().Position(0).CloseButton(false).Dockable(true));
	mAuiManager.AddPane(mActionsView, wxAuiPaneInfo().Name("ActionsListbox").Caption("  Actions").Left().Position(1).CloseButton(false).Dockable(true));
	mAuiManager.AddPane(mOutputView, wxAuiPaneInfo().Name("OutputPane").Caption("  Output").Bottom().Position(1).MaximizeButton(true).CloseButton(false).Dockable(true));
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

wxMenuBar* MainFrame::GetMenuBar()
{
	return mMenubar;
}

SignalScopedConnection MainFrame::DoOnButtonPress(Buttons::ID button,
	ButtonPressSignal::slot_type slot)
{
	return mSignals[button].connect(slot);
}

SignalScopedConnection MainFrame::DoOnHasUnsavedChangesQuery(Signal<bool()>::slot_type slot)
{
	return mHasUnsavedChangesSignal.connect(slot);
}

void MainFrame::SendButtonPressedSignal(Buttons::ID buttonID)
{
	auto found = mSignals.find(buttonID);
	assert(found != mSignals.end());
	found->second();
}

void MainFrame::OpenAboutDialog()
{
	wxAboutDialogInfo info;
	info.SetName("CompilerCreationTool");
	info.SetVersion("1.0.0");
	info.SetDescription("Create your own minimal compiler in just a few clicks!");
	info.SetCopyright("(C) 2019, Timur Karimov <chosti34@gmail.com>");
	wxAboutBox(info, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	boost::optional<bool> hasUnsavedChanges = mHasUnsavedChangesSignal();
	if (hasUnsavedChanges && *hasUnsavedChanges)
	{
		if (wxMessageBox("Current content has not been saved! Proceed?", "Please confirm",
			wxICON_QUESTION | wxYES_NO, this) == wxNO)
		{
			return;
		}
	}
	event.Skip(true);
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
		wxMessageBox("Can't build language: "s + ex.what() + "."s, "Build error", wxICON_WARNING);
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
		wxMessageBox("Can't parse text: "s + ex.what() + "."s, "Can't parse", wxICON_WARNING);
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
		wxMessageBox("Can't get info: "s + ex.what() + "."s, "Can't get info", wxICON_WARNING);
	}
}

void MainFrame::OnHelp(wxCommandEvent&)
{
	OpenAboutDialog();
}

void MainFrame::OnClear(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::Clear);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't clear output: "s + ex.what() + "."s, "Can't clear output", wxICON_WARNING);
	}
}

void MainFrame::OnLogMessages(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::LogMessages);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't change logger state: "s + ex.what() + "."s, "Can't set mask", wxICON_WARNING);
	}
}

void MainFrame::OnCodegenEnable(wxCommandEvent&)
{
	try
	{
		SendButtonPressedSignal(Buttons::EnableCodegen);
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't enable code generation: "s + ex.what());
	}
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
		wxMessageBox(
			"Can't create new language "s + ex.what() + "."s,
			"Can't create new language", wxICON_WARNING);
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
		wxMessageBox(
			"Can't open document: "s + ex.what() + "."s,
			"Can't open document", wxICON_WARNING);
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
		wxMessageBox(
			"Can't save project: "s + ex.what() + "."s,
			"Can't save project", wxICON_WARNING);
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
		wxMessageBox(
			"Can't save document as: "s + ex.what() + "."s,
			"Can't save document as", wxICON_WARNING);
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
		wxMessageBox(
			"Can't move item up: "s + ex.what() + "."s,
			"Can't move item up", wxICON_WARNING);
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
		wxMessageBox(
			"Can't move item up: "s + ex.what() + "."s,
			"Can't move item down", wxICON_WARNING);
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
		wxMessageBox(
			"Can't edit item: "s + ex.what() + "."s,
			"Can't edit item", wxICON_WARNING);
	}
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_SIZE(MainFrame::OnSize)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_MENU(wxID_NEW, MainFrame::OnNew)
	EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
	EVT_MENU(wxID_SAVE, MainFrame::OnSave)
	EVT_MENU(wxID_SAVEAS, MainFrame::OnSaveAs)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)

	EVT_MENU(Buttons::Up, MainFrame::OnItemUp)
	EVT_MENU(Buttons::Down, MainFrame::OnItemDown)
	EVT_MENU(Buttons::Edit, MainFrame::OnItemEdit)
	EVT_MENU(Buttons::Build, MainFrame::OnBuild)
	EVT_MENU(Buttons::Run, MainFrame::OnRun)
	EVT_MENU(Buttons::Info, MainFrame::OnInfo)
	EVT_MENU(Buttons::Clear, MainFrame::OnClear)
	EVT_MENU(Buttons::LogMessages, MainFrame::OnLogMessages)
	EVT_MENU(Buttons::EnableCodegen, MainFrame::OnCodegenEnable)

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
