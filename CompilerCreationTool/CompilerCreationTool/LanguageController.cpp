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

	m_connections.push_back(m_declarationView->DoOnActionPositionChange(
		bind(&LanguageController::OnActionPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_declarationView->DoOnActionEdit(
		bind(&LanguageController::OnActionEdit, this, ph::_1)));

	m_connections.push_back(m_frame->DoOnLanguageBuildButtonPress(
		std::bind(&LanguageController::OnLanguageBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnParserRunButtonPress(
		std::bind(&LanguageController::OnParserRunButtonPress, this)));
	m_connections.push_back(m_frame->DoOnInfoQuery(
		std::bind(&LanguageController::ShowLanguageInfoDialog, this)));
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

	if (noErrors)
	{
		logger->Log("[Successfully parsed!]\n");
	}
	else
	{
		logger->Log("[Failed to parse...]\n");
	}
}

void LanguageController::OnTerminalPositionChange(int oldPos, int newPos)
{
	m_language->GetLexer().SwapPatterns(size_t(oldPos), size_t(newPos));
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
}

void LanguageController::OnActionPositionChange(int oldPos, int newPos)
{
	assert(m_language->IsInitialized());
	m_language->GetParser().SwapActions(size_t(oldPos), size_t(newPos));
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
}

void LanguageController::ShowLanguageInfoDialog()
{
	assert(m_language->IsInitialized());
	LanguageInformationDialog dialog(m_frame, m_language->GetInfo());
	dialog.ShowModal();
}
