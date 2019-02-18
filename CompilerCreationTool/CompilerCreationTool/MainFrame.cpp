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
	Build = 7
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

	toolbar->AddTool(1, wxT("New"), newIcon, wxT("New Document"));
	toolbar->AddTool(2, wxT("Open"), openIcon, wxT("Open Document"));
	toolbar->AddTool(3, wxT("Save"), saveIcon, wxT("Save Document"));
	toolbar->AddTool(4, wxT("Save As"), saveAsIcon, wxT("Save Document As"));
	toolbar->AddSeparator();
	toolbar->AddTool(5, wxT("Undo"), undoIcon, wxT("Undo Command"));
	toolbar->AddTool(6, wxT("Redo"), redoIcon, wxT("Redo Command"));
	toolbar->AddSeparator();
	toolbar->AddTool(Buttons::Build, wxT("Build"), buildIcon, wxT("Build"));
	toolbar->AddTool(8, wxT("Info"), infoIcon, wxT("Get Information"));
	toolbar->AddTool(9, wxT("Help"), helpIcon, wxT("Help"));
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

	GrammarPanel* panel = m_panel->GetGrammarPanel();
	wxListView* list = m_panel->GetTablePanel()->GetListView();

	try
	{
		auto grammar = std::make_unique<grammarlib::Grammar>();
		auto factory = std::make_unique<grammarlib::GrammarProductionFactory>();

		std::string line;
		std::istringstream strm(panel->GetGrammarText());

		while (getline(strm, line))
		{
			auto production = factory->CreateProduction(line);
			grammar->AddProduction(std::move(production));
		}

		m_parser = std::make_unique<Parser>(ParserTable::Create(*grammar));
		const IParserTable& table = m_parser->GetTable();

		list->DeleteAllItems();
		for (size_t i = 0; i < table.GetStatesCount(); ++i)
		{
			long index = list->InsertItem(list->GetItemCount(), std::to_string(i));
			list->SetItem(index, 1, table.GetState(i).GetName());
			list->SetItem(index, 2, table.GetState(i).GetNextAddress() ? std::to_string(*table.GetState(i).GetNextAddress()) : "<none>");
			list->SetItem(index, 3, GetStateFlagsRepresentation(table.GetState(i)));
			if (table.GetState(i).GetFlag(StateFlag::Attribute))
			{
				list->SetItem(index, 4, "<none>");
			}
			else
			{
				list->SetItem(index, 4, string_utils::JoinStrings(*table.GetState(i).GetAcceptableTerminals()));
			}
		}
	}
	catch (const std::exception& ex)
	{
		wxMessageBox("Can't build grammar: " + std::string(ex.what()));
	}
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
	EVT_TOOL(Buttons::Build, MainFrame::OnBuild)
wxEND_EVENT_TABLE()
