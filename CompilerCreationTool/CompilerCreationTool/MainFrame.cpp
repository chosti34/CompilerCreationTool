#include "pch.h"
#include "MainFrame.h"
#include "GrammarPanel.h"
#include "TablePanel.h"
#include "EnvironmentPanel.h"

#include "../Grammar/Grammar.h"
#include "../Grammar/GrammarProduction.h"
#include "../Grammar/GrammarProductionFactory.h"

#include "../Parser/ParserTable.h"
#include "../Parser/ParserState.h"
#include "../Parser/Parser.h"

#include "../Utils/string_utils.h"

#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/listctrl.h>

#include <string>
#include <sstream>

namespace
{
enum Buttons
{
	Build = 7,
	Run = 8,
	Sync = 9
};

std::string GetStateFlagsRepresentation(const IParserState& state)
{
	std::set<std::string> flags;
	for (const StateFlag& flag : GetAllStateFlags())
	{
		if (state.GetFlag(flag))
		{
			flags.insert(ToString(flag));
		}
	}
	return flags.empty() ? "<none>" : string_utils::JoinStrings(flags);
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
	// 1. Достать текст из панели грамматики
	// 2. Создать объекты грамматики и парсера
	// 3. Заполнить таблицу данными парсера

	try
	{
		auto grammar = std::make_unique<grammarlib::Grammar>();
		auto factory = std::make_unique<grammarlib::GrammarProductionFactory>();

		std::string line;
		std::istringstream strm(m_panel->GetGrammarPanel()->GetGrammarText());

		while (getline(strm, line))
		{
			grammar->AddProduction(factory->CreateProduction(line));
		}

		m_compiler->SetLanguageGrammar(std::move(grammar));

		// Заполняем таблицу парсера
		const IParserTable& table = m_compiler->GetParser().GetTable();

		wxListView* parserListView = m_panel->GetTablePanel()->GetListView();
		parserListView->DeleteAllItems();

		for (size_t i = 0; i < table.GetStatesCount(); ++i)
		{
			long index = parserListView->InsertItem(parserListView->GetItemCount(), std::to_string(i));
			parserListView->SetItem(index, 1, table.GetState(i).GetName());
			parserListView->SetItem(index, 2, table.GetState(i).GetNextAddress() ? std::to_string(*table.GetState(i).GetNextAddress()) : "<none>");
			parserListView->SetItem(index, 3, GetStateFlagsRepresentation(table.GetState(i)));
			if (table.GetState(i).GetFlag(StateFlag::Attribute))
			{
				parserListView->SetItem(index, 4, "<none>");
			}
			else
			{
				parserListView->SetItem(index, 4, string_utils::JoinStrings(*table.GetState(i).GetAcceptableTerminals()));
			}
		}

		// Заполняем список терминалов
		const ILexer& lexer = m_compiler->GetLexer();
		wxListBox* terminalsList = m_panel->GetGrammarPanel()->GetTerminalsListBox();
		terminalsList->Clear();

		for (size_t i = 0; i < lexer.GetPatternsCount(); ++i)
		{
			terminalsList->Insert(lexer.GetPattern(i).GetName(), terminalsList->GetCount());
		}
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what());
	}
}

void MainFrame::OnRun(wxCommandEvent& event)
{
	wxStyledTextCtrl* input = m_panel->GetEnvironmentPanel()->GetInputControl();
	wxString text = input->GetValue();

	IParser<bool>& parser = m_compiler->GetParser();

	try
	{
		const bool noErrors = parser.Parse(text.ToStdString());
		if (noErrors)
		{
			wxMessageBox(wxT("Successfully parsed"));
		}
		else
		{
			wxMessageBox(wxT("Syntax error"));
		}
	}
	catch (const std::exception& ex)
	{
		wxMessageBox(ex.what(), wxT("Error"), wxICON_ERROR);
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
	EVT_TOOL(Buttons::Build, MainFrame::OnBuild)
	EVT_TOOL(Buttons::Run, MainFrame::OnRun)
wxEND_EVENT_TABLE()
