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

	/*m_connections.push_back(m_declarationView->DoOnTerminalPositionChange(
		bind(&LanguageController::OnTerminalPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_declarationView->DoOnTerminalEdit(
		bind(&LanguageController::OnTerminalEdit, this, ph::_1)));
	m_connections.push_back(m_declarationView->DoOnTerminalSelection(
		bind(&LanguageController::OnTerminalSelection, this, ph::_1)));*/

	/*m_connections.push_back(m_declarationView->DoOnActionPositionChange(
		bind(&LanguageController::OnActionPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_declarationView->DoOnActionEdit(
		bind(&LanguageController::OnActionEdit, this, ph::_1)));
	m_connections.push_back(m_declarationView->DoOnActionSelection(
		bind(&LanguageController::OnActionSelection, this, ph::_1)));*/

	m_connections.push_back(m_frame->DoOnBuildButtonPress(std::bind(&LanguageController::OnLanguageBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnRunButtonPress(std::bind(&LanguageController::OnParserRunButtonPress, this)));
	m_connections.push_back(m_frame->DoOnInfoButtonPress(std::bind(&LanguageController::OnLanguageInfoButtonPress, this)));

	m_connections.push_back(m_terminalsView->DoOnFocusSet(std::bind(&LanguageController::OnTerminalsViewFocus, this)));
	m_connections.push_back(m_actionsView->DoOnFocusSet(std::bind(&LanguageController::OnActionsViewFocus, this)));

	m_connections.push_back(m_terminalsView->DoOnItemSelection(std::bind(&LanguageController::OnTerminalSelection, this, ph::_1)));
	m_connections.push_back(m_actionsView->DoOnItemSelection(std::bind(&LanguageController::OnActionSelection, this, ph::_1)));

	m_connections.push_back(m_terminalsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnTerminalEdit, this, ph::_1)));
	m_connections.push_back(m_actionsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnActionEdit, this, ph::_1)));

	/*m_connections.push_back(m_editorView->DoOnInputTextCtrlCursorUpdate(
		std::bind(&LanguageController::OnEditorTextCtrlCursorUpdate, this, ph::_1, ph::_2, ph::_3)));
	m_connections.push_back(m_declarationView->DoOnTextCtrlCursorUpdate(
		std::bind(&LanguageController::OnDeclarationTextCtrlCursorUpdate, this, ph::_1, ph::_2, ph::_3)));*/
}

void LanguageController::OnLanguageBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	m_language->SetGrammar(builder->CreateGrammar(m_grammarView->GetDeclaration()));
	m_language->GetParser().SetLogger(std::make_unique<TextCtrlLogger>(m_outputView->GetTextCtrl()));
	m_terminalsView->SetItems(GetTerminalsArray(m_language->GetLexer()));
	m_actionsView->SetItems(GetActionsArray(m_language->GetParser()));
	m_statesView->SetParserTable(m_language->GetParser().GetTable());
}

void LanguageController::OnParserRunButtonPress()
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

void LanguageController::OnLanguageInfoButtonPress()
{
	assert(m_language->IsInitialized());
	LanguageInformationDialog dialog(m_frame, m_language->GetInfo());
	dialog.ShowModal();
}

void LanguageController::OnTerminalsViewFocus()
{
	m_actionsView->DeselectAll();
}

void LanguageController::OnActionsViewFocus()
{
	m_terminalsView->DeselectAll();
}

void LanguageController::OnTerminalPositionChange(int oldPos, int newPos)
{
	m_language->GetLexer().SwapPatterns(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnActionPositionChange(int oldPos, int newPos)
{
	assert(m_language->IsInitialized());
	m_language->GetParser().SwapActions(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnTerminalSelection(int selection)
{
	wxStatusBar* statusbar = m_frame->GetStatusBar();
	statusbar->SetStatusText(m_language->GetLexer().GetPattern(selection).GetOrigin(), 4);
}

void LanguageController::OnActionSelection(int selection)
{
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
		const wxString cTitle = wxT("Information About Terminal");
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
