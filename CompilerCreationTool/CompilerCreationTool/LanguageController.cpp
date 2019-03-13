#include "pch.h"
#include "LanguageController.h"
#include "TerminalEditDialog.h"
#include "ActionEditDialog.h"
#include "TextCtrlLogger.h"
#include "LanguageInformationDialog.h"
#include "../Grammar/GrammarBuilder.h"
#include <functional>

LanguageController::LanguageController(Language* language, MainFrame* frame)
	: m_language(language)
	, m_frame(frame)
	, m_editorView(m_frame->GetMainPanel()->GetCodeEditorView())
	, m_statesView(m_frame->GetMainPanel()->GetParsesStatesView())
	, m_declarationView(m_frame->GetMainPanel()->GetGrammarDeclarationView())
{
	namespace ph = std::placeholders;

	m_connections.push_back(m_declarationView->DoOnTerminalPositionChange(
		bind(&LanguageController::OnTerminalPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_declarationView->DoOnTerminalEdit(
		bind(&LanguageController::OnTerminalEdit, this, ph::_1)));
	m_connections.push_back(m_declarationView->DoOnTerminalSelection(
		bind(&LanguageController::OnTerminalSelection, this, ph::_1)));

	m_connections.push_back(m_declarationView->DoOnActionPositionChange(
		bind(&LanguageController::OnActionPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_declarationView->DoOnActionEdit(
		bind(&LanguageController::OnActionEdit, this, ph::_1)));
	m_connections.push_back(m_declarationView->DoOnActionSelection(
		bind(&LanguageController::OnActionSelection, this, ph::_1)));

	m_connections.push_back(m_frame->DoOnLanguageBuildButtonPress(
		std::bind(&LanguageController::OnLanguageBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnParserRunButtonPress(
		std::bind(&LanguageController::OnParserRunButtonPress, this)));
	m_connections.push_back(m_frame->DoOnInfoQuery(
		std::bind(&LanguageController::OnLanguageInfoButtonPress, this)));

	m_connections.push_back(m_editorView->DoOnInputTextCtrlCursorUpdate(
		std::bind(&LanguageController::OnEditorTextCtrlCursorUpdate, this, ph::_1, ph::_2, ph::_3)));
	m_connections.push_back(m_declarationView->DoOnTextCtrlCursorUpdate(
		std::bind(&LanguageController::OnDeclarationTextCtrlCursorUpdate, this, ph::_1, ph::_2, ph::_3)));
}

void LanguageController::UpdateStatusbarTerminalInfo(int index)
{
	assert(index < m_language->GetLexer().GetPatternsCount());
	wxStatusBar* statusbar = m_frame->GetStatusBar();
	statusbar->SetStatusText(m_language->GetLexer().GetPattern(index).GetOrigin(), 4);
}

void LanguageController::UpdateStatusbarActionInfo(int index)
{
	assert(index < m_language->GetParser().GetActionsCount());
	wxStatusBar* statusbar = m_frame->GetStatusBar();
	statusbar->SetStatusText(ToString(m_language->GetParser().GetAction(index).GetType()), 4);
}

void LanguageController::OnLanguageBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	m_language->SetGrammar(builder->CreateGrammar(m_declarationView->GetDeclaration()));
	m_language->GetParser().SetLogger(std::make_unique<TextCtrlLogger>(m_editorView->GetOutputStyledTextCtrl()));
	m_declarationView->SetLexerTerminals(m_language->GetLexer());
	m_declarationView->SetParserActions(m_language->GetParser());
	m_statesView->SetParserTable(m_language->GetParser().GetTable());
}

void LanguageController::OnParserRunButtonPress()
{
	assert(m_language->IsInitialized());

	IParser<bool>& parser = m_language->GetParser();
	IParserLogger* logger = parser.GetLogger();

	assert(logger);
	logger->Clear();
	logger->Log("[Parsing started]\n");

	const bool noErrors = parser.Parse(m_editorView->GetUserInput().ToStdString());
	logger->Log(noErrors ? "[Successfully parsed!]\n" : "[Failed to parse...]\n");
}

void LanguageController::OnLanguageInfoButtonPress()
{
	assert(m_language->IsInitialized());
	LanguageInformationDialog dialog(m_frame, m_language->GetInfo());
	dialog.ShowModal();
}

void LanguageController::OnTerminalPositionChange(int oldPos, int newPos)
{
	m_language->GetLexer().SwapPatterns(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnTerminalSelection(int selection)
{
	UpdateStatusbarTerminalInfo(selection);
}

void LanguageController::OnTerminalEdit(int index)
{
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
	UpdateStatusbarTerminalInfo(index);
}

void LanguageController::OnActionPositionChange(int oldPos, int newPos)
{
	assert(m_language->IsInitialized());
	m_language->GetParser().SwapActions(size_t(oldPos), size_t(newPos));
}

void LanguageController::OnActionSelection(int selection)
{
	UpdateStatusbarActionInfo(selection);
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
			UpdateStatusbarActionInfo(index);
		}
	}
}

void LanguageController::OnDeclarationTextCtrlCursorUpdate(int line, int col, int ch)
{
	m_frame->GetStatusBar()->SetStatusText("Grammar", 4);
	OnCursorUpdate(line, col, ch);
}

void LanguageController::OnEditorTextCtrlCursorUpdate(int line, int col, int ch)
{
	m_frame->GetStatusBar()->SetStatusText("Editor", 4);
	OnCursorUpdate(line, col, ch);
}

void LanguageController::OnCursorUpdate(int line, int col, int ch)
{
	m_frame->GetStatusBar()->SetStatusText("Ln " + std::to_string(line), 1);
	m_frame->GetStatusBar()->SetStatusText("Col " + std::to_string(col), 2);
	m_frame->GetStatusBar()->SetStatusText("Ch " + std::to_string(ch), 3);
}
