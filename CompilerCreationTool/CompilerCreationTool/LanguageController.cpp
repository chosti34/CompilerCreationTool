#include "pch.h"
#include "LanguageController.h"
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
		std::bind(&LanguageController::OnTerminalPositionChange, this, ph::_1, ph::_2)));
	m_connections.push_back(m_frame->DoOnLanguageBuildButtonPress(
		std::bind(&LanguageController::OnLanguageBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnParserRunButtonPress(
		std::bind(&LanguageController::OnParserRunButtonPress, this)));
}

void LanguageController::OnLanguageBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	m_language->SetLanguageGrammar(builder->CreateGrammar(m_declarationView->GetDeclaration()));

	m_declarationView->SetLexerTerminals(m_language->GetLexer());
	m_statesView->SetParserTable(m_language->GetParser().GetTable());
}

void LanguageController::OnParserRunButtonPress()
{
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
