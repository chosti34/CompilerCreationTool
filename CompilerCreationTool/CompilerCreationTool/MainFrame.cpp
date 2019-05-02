#include "pch.h"
#include "MainFrame.h"
#include <wx/artprov.h>
#include <wx/aui/auibar.h>
#include <wx/aui/dockart.h>

using namespace std::literals::string_literals;

static const char* settings_xpm[] = {
	/* columns rows colors chars-per-pixel */
	"24 24 187 2 ",
	"   c #060E14",
	".  c #120E12",
	"X  c #160D12",
	"o  c #170F14",
	"O  c #180C12",
	"+  c #1B0E14",
	"@  c #071015",
	"#  c #071017",
	"$  c #071117",
	"%  c #0C1314",
	"&  c #0D1315",
	"*  c #081218",
	"=  c #09131B",
	"-  c #161317",
	";  c #1E1319",
	":  c #1E161C",
	">  c #101B1D",
	",  c #111C1D",
	"<  c #121D1F",
	"1  c #1E191F",
	"2  c #221119",
	"3  c #241019",
	"4  c #25121B",
	"5  c #25141C",
	"6  c #2A121D",
	"7  c #2E141F",
	"8  c #0B1A23",
	"9  c #0E1A21",
	"0  c #0E1C26",
	"q  c #0D1F2B",
	"w  c #121F21",
	"e  c #1E1A20",
	"r  c #1F1E24",
	"t  c #201B21",
	"y  c #241B22",
	"u  c #351422",
	"i  c #371E2B",
	"p  c #122226",
	"a  c #122227",
	"s  c #142125",
	"d  c #142225",
	"f  c #152325",
	"g  c #162527",
	"h  c #132229",
	"j  c #15272C",
	"k  c #17272E",
	"l  c #192B2E",
	"z  c #152833",
	"x  c #262128",
	"c  c #23272D",
	"v  c #3E202E",
	"b  c #292B32",
	"n  c #2E2B34",
	"m  c #2C2F38",
	"M  c #322631",
	"N  c #3D2230",
	"B  c #411828",
	"V  c #511C31",
	"C  c #511E33",
	"Z  c #581E35",
	"A  c #502539",
	"S  c #53253A",
	"D  c #5F213A",
	"F  c #55283C",
	"G  c #183649",
	"H  c #323940",
	"J  c #333B43",
	"K  c #303F46",
	"L  c #3D3D49",
	"P  c #6D2541",
	"I  c #612941",
	"U  c #642A43",
	"Y  c #6F2B47",
	"T  c #702E4A",
	"R  c #7B2948",
	"E  c #2E4349",
	"W  c #3D414C",
	"Q  c #2D4D53",
	"!  c #284B54",
	"~  c #2C4C55",
	"^  c #384750",
	"/  c #3D4550",
	"(  c #3B4850",
	")  c #394D54",
	"_  c #3D4D58",
	"`  c #3B595F",
	"'  c #214B69",
	"]  c #414F5B",
	"[  c #4A6772",
	"{  c #50727B",
	"}  c #822B4D",
	"|  c #852B4E",
	" . c #872C4F",
	".. c #8D3156",
	"X. c #923359",
	"o. c #983259",
	"O. c #9D345C",
	"+. c #9F345D",
	"@. c #A33661",
	"#. c #A53661",
	"$. c #A53761",
	"%. c #A73762",
	"&. c #B03A67",
	"*. c #B33A69",
	"=. c #B43B69",
	"-. c #B53B6A",
	";. c #BF3E70",
	":. c #C34072",
	">. c #C44073",
	",. c #C94276",
	"<. c #D2457B",
	"1. c #D4457C",
	"2. c #D9477F",
	"3. c #285A80",
	"4. c #2B6288",
	"5. c #2F6C96",
	"6. c #316D94",
	"7. c #32739F",
	"8. c #377EAE",
	"9. c #377FAF",
	"0. c #497E87",
	"q. c #4F7D88",
	"w. c #507883",
	"e. c #DA4780",
	"r. c #DC4881",
	"t. c #DF4982",
	"y. c #DF4983",
	"u. c #E24A84",
	"i. c #E24A85",
	"p. c #E34A85",
	"a. c #3A86B7",
	"s. c #3A86B8",
	"d. c #3D8ABD",
	"f. c #3C8BBF",
	"g. c #4D868F",
	"h. c #53838E",
	"j. c #4A8490",
	"k. c #4F8992",
	"l. c #538995",
	"z. c #538F99",
	"x. c #52909C",
	"c. c #56949E",
	"v. c #58949E",
	"b. c #5B94A0",
	"n. c #5F9BA5",
	"m. c #589BA8",
	"M. c #5B9DA8",
	"N. c #5B9EA9",
	"B. c #6397A1",
	"V. c #629AA5",
	"C. c #609AA6",
	"Z. c #649EAB",
	"A. c #65A2AF",
	"S. c #6CA6B0",
	"D. c #6FAAB5",
	"F. c #6CABB8",
	"G. c #66B1BD",
	"H. c #73B0BB",
	"J. c #73B1BC",
	"K. c #3E8EC2",
	"L. c #3F8FC4",
	"P. c #3F8FCA",
	"I. c #4090CB",
	"U. c #4296CE",
	"Y. c #4296CF",
	"T. c #4297CF",
	"R. c #66B3C0",
	"E. c #69B6C2",
	"W. c #69B7C3",
	"Q. c #6CBAC7",
	"!. c #6DBFCC",
	"~. c #73B5C2",
	"^. c #77B7C3",
	"/. c #71B8C4",
	"(. c #72BAC6",
	"). c #75B8C5",
	"_. c #79BBC8",
	"`. c #70C1CE",
	"'. c #70C2CF",
	"]. c #7DC0CF",
	"[. c #7FC3D0",
	"{. c #7AC7D4",
	"}. c #80C4D0",
	"|. c #80C5D3",
	" X c #82C7D4",
	".X c #85CCD9",
	"XX c None",
	/* pixels */
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"# q G G G G G G G G G G G G G G G G G = XXXXXXXX",
	"= 5.7.' 8.4.T.T.T.T.T.T.T.T.T.T.T.T.T.a.# XXXXXX",
	"8 f.f.8.I.s.T.T.T.T.T.T.T.T.T.T.T.T.T.T.XXXXXXXX",
	"8 U.U.6.h k k k k k k k k k k k k k k k f f < XX",
	"8 U.U.z C.C.b._.l..X.X.X.X.X.X.X.X.X.X.X{.!.G., ",
	"8 T.I.0 ~.C.b._.l..X.X.X.X.X.X.X.X.X.X{.'.'.!.d ",
	"8 T.I.0 Z.a a a a a a a a a a a a a a a a a j.< ",
	"8 T.P.0 .X.X.X.X.X.X.X.XJ.S.D..X.X{.'.'.'.'.'.d ",
	"8 T.P.0 Z.a a k .X}.~. X. F 2 J./.W.'.'.'.'.'.< ",
	"8 T.P.0 ].A.A.}.}.b : ) v p.I E 1 r G.'.'.'.'.< ",
	"8 T.P.0 F.~ ~ ).H ..,.O.u.p.u.#.;.$.y '.'.'.'.d ",
	"8 T.P.0 .X.X.X.X{ A p.p.;.Y =.p.u.P _ !.'.'.'.< ",
	"8 T.P.0 .X.X.XB.` U p.T H k.^ S 2.R E g.'.'.'.< ",
	"8 T.P.0 .X.X.X1  .1.r.x '.'.'.M >.1. .. '.'.'.d ",
	"8 T.P.0 .X.X.Xt p.p.,.m '.'.'.W %.2.2.5 '.'.!.d ",
	"8 T.I.0 .X.X.X^ - X.p.v z.'.N.7 2.@.X n !.'.'.< ",
	"8 T.I.0 .X.X.X.XC.i p.:.D 5 C *.2.Z j.'.`.'.'.< ",
	"# K.P.0 .X.X.X{.c +.2.2.e.2.2.2.2.*.o '.'.'.'.d ",
	"XX  3.8 .X.X{.'.c.6 V + } 2.o.2 B u q.'.'.'.'.< ",
	"XXXXXX& .X{.'.'.'.v.[ W.3 &.B N.w.h.'.'.m.m.G.d ",
	"XXXXXX& ).'.'.'.'.'.'.'.] L / Q.'.'.'.!.! ! z., ",
	"XXXXXXXXf Q Q Q Q Q Q Q Q Q Q Q Q Q Q Q Q Q l f ",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

static const char* build_xpm[] = {
	/* columns rows colors chars-per-pixel */
	"24 24 67 1 ",
	"  c #5391CD",
	". c #5492CD",
	"X c #5492CE",
	"o c #5493CE",
	"O c #5694CE",
	"+ c #5694CF",
	"@ c #5794CF",
	"# c #5695CF",
	"$ c #5895CF",
	"% c #5796D0",
	"& c #5895D0",
	"* c #5996D1",
	"= c #5A97D1",
	"- c #5B98D1",
	"; c #5C98D1",
	": c #5D98D1",
	"> c #5D9AD1",
	", c #5D99D2",
	"< c #5D99D3",
	"1 c #5E99D2",
	"2 c #5F9AD2",
	"3 c #5F9AD3",
	"4 c #609AD2",
	"5 c #609BD3",
	"6 c #619CD3",
	"7 c #619DD3",
	"8 c #629CD4",
	"9 c #639DD4",
	"0 c #629DD6",
	"q c #649ED4",
	"w c #659ED5",
	"e c #67A0D7",
	"r c #69A1D7",
	"t c #6AA3D7",
	"y c #67A0D8",
	"u c #69A1D8",
	"i c #69A2D8",
	"p c #6AA3D8",
	"a c #6BA2DA",
	"s c #6BA3DA",
	"d c #6BA4D8",
	"f c #6CA4DA",
	"g c #6FA6DA",
	"h c #6FA6DB",
	"j c #6FA7DC",
	"k c #70A7DC",
	"l c #72A9DC",
	"z c #72A8DD",
	"x c #74AADD",
	"c c #75ABDE",
	"v c #7BB0E1",
	"b c #81B4E4",
	"n c #85B7E6",
	"m c #86B7E7",
	"M c #86B8E7",
	"N c #87B9E7",
	"B c #88B9E7",
	"V c #89BAE8",
	"C c #8CBBE9",
	"Z c #8EBDEA",
	"A c #90BFEB",
	"S c #93C1EC",
	"D c #95C3ED",
	"F c #A0CBF3",
	"G c #A3CDF4",
	"H c #B6DCFE",
	"J c None",
	/* pixels */
	"JJJJJJJJJJJJJJJJJJJJJJJJ",
	"J 668=6666e66,#6666666#J",
	"J,HHHcHHHHHHHGkHHHHHHH6J",
	"J,HHHxHHHHHHHGkHHHHHHH3J",
	"J,HHHxHHHHHHHGkHHHHHHH5J",
	"J$xxxq886,6666,688txxz+J",
	"J,HHHHHHHzGHHHHHHHxHHH5J",
	"J,HHHHHHHkGHHHHHHHxHHH,J",
	"J-HHHHHHHkGHHHHHHHxHHH5J",
	"J#CBMvzzz,szzjszzzsVVV=J",
	"J=FFGhGFFFFFGZsFFFFFFF;J",
	"J,HHHxHHHHHHHGhHHHHHHH,J",
	"J,HHHzHHHHHHHGhHHHHHHH,J",
	"J=FFFhZZZMCZZb8ZZZDFFF,J",
	"J+BVBMMMMszMMmMMnmtVVV=J",
	"J,HHHHHHHnSHHHHHHHxHHH5J",
	"J,HHHHHHHnSHHHHHHHcHHH5J",
	"J,HHHHHHHnSHHHHHHHxHHH5J",
	"J$xxx5dddtsdd6#dddkxxx+J",
	"J,HHHxHHHHHHHGkHHHHHHH,J",
	"J,HHHcHHHHHHHGkHHHHHHH5J",
	"J,HHHxHHHHHHHGkHHHHHHH,J",
	"J+qqq,qqqqqqq5=8qqqqqq%J",
	"JJJJJJJJJJJJJJJJJJJJJJJJ"
};

static const char* play_xpm[] = {
	/* columns rows colors chars-per-pixel */
	"24 24 14 1 ",
	"  c #8AC249",
	". c #8AC34A",
	"X c #8BC34A",
	"o c #8BC34B",
	"O c #91C653",
	"+ c #95C85A",
	"@ c #A3CF6F",
	"# c #C0DE9C",
	"$ c #D5E9BD",
	"% c #DFEECD",
	"& c #EFF7E5",
	"* c #FDFEFB",
	"= c white",
	"- c None",
	/* pixels */
	"------------------------",
	"------------------------",
	"----XXXXXXXXXXXXXXXX----",
	"---XXXXXXXXXXXXXXXXXX---",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"--XXXXXXX=XXXXXXXXXXXX--",
	"--XXXXXXX==$OXXXXXXXXX--",
	"--XXXXXXX===&@XXXXXXXX--",
	"--XXXXXXX====*#XXXXXXX--",
	"--XXXXXXX======%+XXXXX--",
	"--XXXXXXX======%+XXXXX--",
	"--XXXXXXX====*#XXXXXXX--",
	"--XXXXXXX===&@XXXXXXXX--",
	"--XXXXXXX==$OXXXXXXXXX--",
	"--XXXXXXX=XXXXXXXXXXXX--",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"--XXXXXXXXXXXXXXXXXXXX--",
	"---XXXXXXXXXXXXXXXXXX---",
	"----XXXXXXXXXXXXXXXX----",
	"------------------------",
	"------------------------"
};

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
	edit->Append(0, "Move item up");
	edit->Append(1, "Move item down");
	edit->Append(2, "Item settings...");

	wxMenu* view = new wxMenu;
	view->AppendCheckItem(
		Buttons::LogMessages, "Log only action's messages",
		"Check this item if you want to log only action's messages");
	view->AppendSeparator();
	view->Append(Buttons::Clear, "Clear output", nullptr, "Clear output window's content");

	wxMenu* language = new wxMenu;
	language->Append(0, "Build");
	language->Append(1, "Run");
	language->Append(2, "Get info");

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

	//wxIcon buildIcon = wxArtProvider::GetIcon(wxART_PASTE, wxART_OTHER, iconSize);
	wxIcon buildIcon(build_xpm);
	//wxIcon runIcon = wxArtProvider::GetIcon(wxART_COPY, wxART_OTHER, iconSize);
	wxIcon runIcon(play_xpm);
	wxIcon infoIcon = wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, iconSize);

	wxIcon upIcon = wxArtProvider::GetIcon(wxART_GO_UP, wxART_OTHER, iconSize);
	wxIcon downIcon = wxArtProvider::GetIcon(wxART_GO_DOWN, wxART_OTHER, iconSize);
	//wxIcon editIcon = wxArtProvider::GetIcon(wxART_FULL_SCREEN, wxART_OTHER, iconSize);
	wxIcon editIcon(settings_xpm);

	wxIcon helpIcon = wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, iconSize);

	toolbar.AddTool(Buttons::New, wxT("New"), newIcon, wxT("New document"));
	toolbar.AddTool(Buttons::Open, wxT("Open"), openIcon, wxT("Open document"));
	toolbar.AddTool(Buttons::Save, wxT("Save"), saveIcon, wxT("Save document"));
	toolbar.AddTool(Buttons::SaveAs, wxT("Save As"), saveAsIcon, wxT("Save document as"));
	toolbar.AddSeparator();

	//toolbar.AddTool(Buttons::Undo, wxT("Undo"), undoIcon, wxT("Undo command"));
	//toolbar.AddTool(Buttons::Redo, wxT("Redo"), redoIcon, wxT("Redo command"));
	//toolbar.AddSeparator();

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
	// Menubar
	mMenubar = CreateMenuBar();
	SetMenuBar(mMenubar);

	// Toolbar
	mToolbar = CreateToolBar(wxTB_FLAT);
	AddTools(*mToolbar);
	mToolbar->Realize();

	mMenubar->Enable(Buttons::LogMessages, false);
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
	wxMessageBox(
		"Tool for building compiler.",
		"About CompilerCreationTool",
		wxOK | wxICON_INFORMATION);
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
	EVT_MENU(wxID_NEW, MainFrame::OnNew)
	EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
	EVT_MENU(wxID_SAVE, MainFrame::OnSave)
	EVT_MENU(wxID_SAVEAS, MainFrame::OnSaveAs)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(Buttons::Clear, MainFrame::OnClear)
	EVT_MENU(Buttons::LogMessages, MainFrame::OnLogMessages)
	EVT_SIZE(MainFrame::OnSize)
	EVT_CLOSE(MainFrame::OnClose)
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
