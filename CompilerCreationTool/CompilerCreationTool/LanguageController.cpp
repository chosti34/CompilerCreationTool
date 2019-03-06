#include "pch.h"
#include "LanguageController.h"
#include "TerminalEditDialog.h"
#include "ActionEditDialog.h"
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
	m_declarationView->SetLexerTerminals(m_language->GetLexer());
	m_declarationView->SetParserActions(m_language->GetParser());
	m_statesView->SetParserTable(m_language->GetParser().GetTable());
}

void LanguageController::OnParserRunButtonPress()
{
	if (!m_language->IsInitialized())
	{
		wxMessageBox(
			"Declare language before trying to parse text.",
			"Error!",
			wxICON_WARNING
		);
		return;
	}

	IParser<bool>& parser = m_language->GetParser();
	const bool noErrors = parser.Parse(m_editorView->GetUserInput().ToStdString());

	if (noErrors)
	{
		wxMessageBox(wxT("Successfully parsed"));
	}
	else
	{
		wxMessageBox(wxT("Syntax error"));
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
		wxMessageBox(
			"You can't edit grammar's ending terminal.\n"
			"Please, choose another one.",
			"Information about terminal",
			wxOK | wxICON_INFORMATION);
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
	if (!m_language->IsInitialized())
	{
		wxMessageBox(
			wxT("Language is not initialized yet"),
			wxT("Can't show info"),
			wxICON_WARNING
		);
		return;
	}

	LanguageInformationDialog dialog(m_frame, m_language->GetInfo());
	dialog.ShowModal();
}
