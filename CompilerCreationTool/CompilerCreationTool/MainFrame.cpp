#include "pch.h"
#include "MainFrame.h"
#include "GrammarPanel.h"
#include "TablePanel.h"
#include "EnvironmentPanel.h"

#include "../Grammar/GrammarBuilder.h"
#include "../Parser/ParserTable.h"
#include "../Parser/ParserState.h"
#include "../Parser/Parser.h"
#include "../Utils/string_utils.h"

#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/listctrl.h>

namespace
{
enum Buttons
{
	Build = 7,
	Run = 8,
	Sync = 9
};

void RefillParserTableView(wxListView& tableView, const IParserTable& parserTable)
{
	tableView.DeleteAllItems();

	const std::string noFlagsRepresentation = "<none>";
	const auto getStateFlagsRepresentation = [&](const IParserState& state) {
		std::set<std::string> flags;
		for (const StateFlag& flag : GetAllStateFlags())
		{
			if (state.GetFlag(flag))
			{
				flags.insert(ToString(flag));
			}
		}
		return flags.empty() ? noFlagsRepresentation : string_utils::JoinStrings(flags);
	};

	for (size_t i = 0; i < parserTable.GetStatesCount(); ++i)
	{
		// Insert row (and specifying first column!)
		long rowIndex = tableView.InsertItem(
			tableView.GetItemCount(), std::to_string(i));

		// Fill columns
		tableView.SetItem(rowIndex, 1, parserTable.GetState(i).GetName());
		tableView.SetItem(rowIndex, 2, parserTable.GetState(i).GetNextAddress() ?
			std::to_string(*parserTable.GetState(i).GetNextAddress()) : "<none>");
		tableView.SetItem(rowIndex, 3, getStateFlagsRepresentation(parserTable.GetState(i)));

		if (parserTable.GetState(i).GetFlag(StateFlag::Attribute))
		{
			tableView.SetItem(rowIndex, 4, noFlagsRepresentation);
		}
		else
		{
			tableView.SetItem(rowIndex, 4,
				string_utils::JoinStrings(*parserTable.GetState(i).GetAcceptableTerminals()));
		}
	}
}

void RefillTerminalsListbox(wxListBox& listbox, const ILexer& lexer)
{
	listbox.Clear();
	for (size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		listbox.Insert(lexer.GetPattern(i).GetName(), listbox.GetCount());
	}
}

void DoExceptionSafely(std::function<void()> && action)
{
	try
	{
		action();
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what());
	}
}
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
	, m_panel(new MainPanel(this))
	, m_compiler(std::make_unique<Compiler>())
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
	wxIcon openIcon = wxArtProvider::GetIcon(wxART_FILE_OPEN, wxART_OTHER, wxSize(24, 24));
	wxIcon undoIcon = wxArtProvider::GetIcon(wxART_UNDO, wxART_OTHER, wxSize(24, 24));
	wxIcon redoIcon = wxArtProvider::GetIcon(wxART_REDO, wxART_OTHER, wxSize(24, 24));
	wxIcon helpIcon = wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, wxSize(24, 24));
	wxIcon fileIcon = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_OTHER, wxSize(24, 24));

	toolbar->AddTool(1, wxT("New"), newIcon, wxT("New Document"));
	toolbar->AddTool(2, wxT("Open"), openIcon, wxT("Open Document"));
	toolbar->AddTool(3, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar->AddTool(4, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar->AddSeparator();
	toolbar->AddTool(5, wxT("Undo"), undoIcon, wxT("Undo Command"));
	toolbar->AddTool(6, wxT("Redo"), redoIcon, wxT("Redo Command"));
	toolbar->AddSeparator();
	toolbar->AddTool(Buttons::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar->AddTool(Buttons::Run, wxT("Run"), fileIcon, wxT("Run"));
	toolbar->AddTool(9, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar->AddTool(10, wxT("Help"), helpIcon, wxT("Help"));
	toolbar->Realize();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticLine* line = new wxStaticLine(this, wxID_ANY);
	sizer->Add(line, 0, wxEXPAND);
	sizer->Add(m_panel, 1, wxEXPAND);
	SetSizer(sizer);

	CreateStatusBar();
	SetStatusText(wxT("Welcome to CompilerCreationTool!"));

	Centre();
	m_panel->GetTablePanel()->AdjustColumnWidths();
	m_panel->GetEnvironmentPanel()->Split();
	m_panel->GetGrammarPanel()->Split();

	m_connections.push_back(m_panel->GetGrammarPanel()->RegisterOnTerminalPositionChangedCallback([this](int oldPos, int newPos) {
		m_compiler->GetLexer().SwapPatterns(oldPos, newPos);
	}));
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
	DoExceptionSafely([this]() {
		auto builder = std::make_unique<grammarlib::GrammarBuilder>();

		m_compiler->SetLanguageGrammar(
			builder->CreateGrammar(m_panel->GetGrammarPanel()->GetGrammarText()));

		RefillParserTableView(
			*m_panel->GetTablePanel()->GetListView(),
			m_compiler->GetParser().GetTable()
		);

		RefillTerminalsListbox(
			*m_panel->GetGrammarPanel()->GetTerminalsListBox(),
			m_compiler->GetLexer()
		);
	});
}

void MainFrame::OnRun(wxCommandEvent& event)
{
	DoExceptionSafely([this]() {
		wxStyledTextCtrl* input = m_panel->GetEnvironmentPanel()->GetInputControl();
		wxString text = input->GetValue();

		IParser<bool>& parser = m_compiler->GetParser();
		const bool noErrors = parser.Parse(text.ToStdString());

		if (noErrors)
		{
			wxMessageBox(wxT("Successfully parsed"));
		}
		else
		{
			wxMessageBox(wxT("Syntax error"));
		}
	});
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TOOL(Buttons::Build, MainFrame::OnBuild)
	EVT_TOOL(Buttons::Run, MainFrame::OnRun)
wxEND_EVENT_TABLE()
