#include "pch.h"
#include "LanguageController.h"
#include "TerminalEditDialog.h"
#include "ActionEditDialog.h"
#include "TextCtrlLogger.h"
#include "LanguageInformationDialog.h"
#include "ASTGraphvizVisualizer.h"
#include "../Grammar/GrammarBuilder.h"
#include "../Utils/command_utils.h"
#include "../Utils/time_utils.h"
#include <functional>

namespace
{
wxArrayString GetTerminalsArray(const ILexer& lexer)
{
	wxArrayString arr;
	arr.reserve(lexer.GetPatternsCount());
	for (std::size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		arr.push_back(lexer.GetPattern(i).GetName());
	}
	return arr;
}

wxArrayString GetActionsArray(const IParser<ParseResults>& parser)
{
	wxArrayString arr;
	arr.reserve(parser.GetActionsCount());
	for (std::size_t i = 0; i < parser.GetActionsCount(); ++i)
	{
		arr.push_back(parser.GetAction(i).GetName());
	}
	return arr;
}
}

LanguageController::LanguageController(Language* language, MainFrame* frame)
	: m_language(language)
	, m_frame(frame)
	, m_grammarView(m_frame->GetDeclarationView())
	, m_statesView(m_frame->GetStatesView())
	, m_editorView(m_frame->GetEditorView())
	, m_treeView(m_frame->GetTreeView())
	, m_terminalsView(m_frame->GetTerminalsView())
	, m_actionsView(m_frame->GetActionsView())
	, m_outputView(m_frame->GetOutputView())
{
	namespace ph = std::placeholders;

	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Build, std::bind(&LanguageController::OnBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Run, std::bind(&LanguageController::OnRunButtonPress, this)));
	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Info, std::bind(&LanguageController::OnInfoButtonPress, this)));

	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Up, std::bind(&LanguageController::OnUpButtonPress, this)));
	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Down, std::bind(&LanguageController::OnDownButtonPress, this)));
	m_connections.push_back(m_frame->DoOnButtonPress(Buttons::Edit, std::bind(&LanguageController::OnEditButtonPress, this)));

	m_connections.push_back(m_terminalsView->DoOnItemDeselection(std::bind(&LanguageController::OnTerminalsViewDeselection, this)));
	m_connections.push_back(m_actionsView->DoOnItemDeselection(std::bind(&LanguageController::OnActionsViewDeselection, this)));

	m_connections.push_back(m_terminalsView->DoOnItemSelection(std::bind(&LanguageController::OnTerminalSelection, this, ph::_1)));
	m_connections.push_back(m_actionsView->DoOnItemSelection(std::bind(&LanguageController::OnActionSelection, this, ph::_1)));

	m_connections.push_back(m_terminalsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnTerminalEdit, this, ph::_1)));
	m_connections.push_back(m_actionsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnActionEdit, this, ph::_1)));

	m_connections.push_back(m_grammarView->DoOnTextCtrlUpdateUI(std::bind(&LanguageController::OnGrammarTextCtrlUpdateUI, this, ph::_1, ph::_2, ph::_3)));
	m_connections.push_back(m_editorView->DoOnTextCtrlUpdateUI(std::bind(&LanguageController::OnEditorTextCtrlUpdateUI, this, ph::_1, ph::_2, ph::_3)));
}

void LanguageController::OnBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	m_language->SetGrammar(builder->CreateGrammar(m_grammarView->GetDeclaration()));
	m_language->GetParser().SetLogger(std::make_unique<TextCtrlLogger>(m_outputView->GetTextCtrl()));
	m_terminalsView->SetItems(GetTerminalsArray(m_language->GetLexer()));
	m_actionsView->SetItems(GetActionsArray(m_language->GetParser()));
	m_statesView->SetParserTable(m_language->GetParser().GetTable());
	m_frame->GetToolBar()->EnableTool(Buttons::Run, true);
	m_frame->GetToolBar()->EnableTool(Buttons::Info, true);
}

void LanguageController::OnRunButtonPress()
{
	assert(m_language->IsInitialized());

	IParser<ParseResults>& parser = m_language->GetParser();
	IParserLogger* logger = parser.GetLogger();
	assert(logger);

	logger->Log("[" + time_utils::GetCurrentTimeAsString() + "] Parsing started...\n");
	const ParseResults results = parser.Parse(m_editorView->GetUserInput().ToStdString());
	logger->Log(results.success ? "Successfully parsed!\n" : "Failed to parse...\n");

	if (results.success && results.expression)
	{
		{
			// Explicitly close file 'ast.dot'
			ASTGraphvizVisualizer visualizer("ast.dot");
			visualizer.Visualize(*results.expression);
		}

		if (command_utils::RunCommand(L"dot", L"-T png -o ast.png ast.dot"))
		{
			wxImage img;
			img.LoadFile("ast.png");

			if (img.IsOk())
			{
				m_treeView->SetImage(img);
				logger->Log("AST has been drawn!\n");
			}
			else
			{
				logger->Log("Can't draw AST...\n");
			}
		}
		else
		{
			logger->Log("Install Graphviz package to draw AST...\n");
		}
	}
	else
	{
		m_treeView->UnsetImage();
	}

	logger->Log("=========================\n");
}

void LanguageController::OnInfoButtonPress()
{
	assert(m_language->IsInitialized());
	LanguageInformationDialog dialog(m_frame, m_language->GetInfo());
	dialog.ShowModal();
}

void LanguageController::OnUpButtonPress()
{
	if (m_terminalsView->HasSelection() && m_terminalsView->MoveSelectionUp())
	{
		OnTerminalPositionChange(m_terminalsView->GetSelection(), m_terminalsView->GetSelection() + 1);
	}
	else if (m_actionsView->HasSelection() && m_actionsView->MoveSelectionUp())
	{
		OnActionPositionChange(m_actionsView->GetSelection(), m_actionsView->GetSelection() + 1);
	}
}

void LanguageController::OnDownButtonPress()
{
	if (m_terminalsView->HasSelection() && m_terminalsView->MoveSelectionDown())
	{
		OnTerminalPositionChange(m_terminalsView->GetSelection(), m_terminalsView->GetSelection() - 1);
	}
	else if (m_actionsView->HasSelection() && m_actionsView->MoveSelectionDown())
	{
		OnActionPositionChange(m_terminalsView->GetSelection(), m_terminalsView->GetSelection() - 1);
	}
}

void LanguageController::OnEditButtonPress()
{
	if (m_terminalsView->HasSelection())
	{
		OnTerminalEdit(m_terminalsView->GetSelection());
	}
	else if (m_actionsView->HasSelection())
	{
		OnActionEdit(m_actionsView->GetSelection());
	}
}

void LanguageController::OnTerminalsViewDeselection()
{
	std::cout << "Term deselection" << std::endl;
	m_frame->GetToolBar()->EnableTool(Buttons::Up, false);
	m_frame->GetToolBar()->EnableTool(Buttons::Down, false);
	m_frame->GetToolBar()->EnableTool(Buttons::Edit, false);
	m_actionsView->DeselectAll();
}

void LanguageController::OnActionsViewDeselection()
{
	std::cout << "Action deselection" << std::endl;
	m_frame->GetToolBar()->EnableTool(Buttons::Up, false);
	m_frame->GetToolBar()->EnableTool(Buttons::Down, false);
	m_frame->GetToolBar()->EnableTool(Buttons::Edit, false);
	m_terminalsView->DeselectAll();
}

void LanguageController::OnTerminalPositionChange(int oldPos, int newPos)
{
	assert(m_language->IsInitialized());
	m_language->GetLexer().SwapPatterns(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnActionPositionChange(int oldPos, int newPos)
{
	assert(m_language->IsInitialized());
	m_language->GetParser().SwapActions(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnTerminalSelection(int selection)
{
	m_actionsView->DeselectAll();
	m_frame->GetToolBar()->EnableTool(Buttons::Up, true);
	m_frame->GetToolBar()->EnableTool(Buttons::Down, true);
	m_frame->GetToolBar()->EnableTool(Buttons::Edit, true);
	wxStatusBar* statusbar = m_frame->GetStatusBar();
	statusbar->SetStatusText(m_language->GetLexer().GetPattern(selection).GetOrigin(), 4);
}

void LanguageController::OnActionSelection(int selection)
{
	m_terminalsView->DeselectAll();
	m_frame->GetToolBar()->EnableTool(Buttons::Up, true);
	m_frame->GetToolBar()->EnableTool(Buttons::Down, true);
	m_frame->GetToolBar()->EnableTool(Buttons::Edit, true);
	wxStatusBar* statusbar = m_frame->GetStatusBar();
	statusbar->SetStatusText(ToString(m_language->GetParser().GetAction(selection).GetType()), 4);
}

void LanguageController::OnTerminalEdit(int index)
{
	assert(m_language->IsInitialized());
	assert(index < m_language->GetLexer().GetPatternsCount());

	TokenPattern& pattern = m_language->GetLexer().GetPattern(index);
	if (pattern.IsEnding())
	{
		using namespace std::string_literals;
		const wxString cTitle = wxT("Information About Terminal '" + pattern.GetName() + "'");
		const wxString cMessage = "'"s + pattern.GetName() +
			"' is grammar's ending terminal - you cannot edit it."s;
		wxMessageBox(cMessage, cTitle, wxOK | wxICON_WARNING);
		return;
	}

	TerminalEditDialog dialog(m_frame, pattern);
	dialog.ShowModal();

	OnTerminalSelection(index);
}

void LanguageController::OnActionEdit(int index)
{
	assert(m_language->IsInitialized());
	assert(index < m_language->GetParser().GetActionsCount());

	IAction& action = m_language->GetParser().GetAction(index);
	ActionEditDialog dialog(m_frame, action);

	if (dialog.ShowModal() == wxID_OK)
	{
		const ActionType newActionType = dialog.GetActionTypeSelection();
		if (action.GetType() != newActionType)
		{
			action.SetType(newActionType);
		}
	}

	OnActionSelection(index);
}

void LanguageController::OnGrammarTextCtrlUpdateUI(int line, int col, int ch)
{
	m_frame->GetStatusBar()->SetStatusText("Grammar", 4);
	m_frame->GetStatusBar()->SetStatusText("Ln " + std::to_string(line), 1);
	m_frame->GetStatusBar()->SetStatusText("Col " + std::to_string(col), 2);
	m_frame->GetStatusBar()->SetStatusText("Ch " + std::to_string(ch), 3);
}

void LanguageController::OnEditorTextCtrlUpdateUI(int line, int col, int ch)
{
	m_frame->GetStatusBar()->SetStatusText("Editor", 4);
	m_frame->GetStatusBar()->SetStatusText("Ln " + std::to_string(line), 1);
	m_frame->GetStatusBar()->SetStatusText("Col " + std::to_string(col), 2);
	m_frame->GetStatusBar()->SetStatusText("Ch " + std::to_string(ch), 3);
}
