#include "pch.h"
#include "LanguageController.h"
#include "TerminalEditDialog.h"
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
	m_connections.push_back(m_frame->DoOnLanguageBuildButtonPress(
		std::bind(&LanguageController::OnLanguageBuildButtonPress, this)));
	m_connections.push_back(m_frame->DoOnParserRunButtonPress(
		std::bind(&LanguageController::OnParserRunButtonPress, this)));
}

void LanguageController::OnLanguageBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	m_language->SetGrammar(builder->CreateGrammar(m_declarationView->GetDeclaration()));
	m_declarationView->SetLexerTerminals(m_language->GetLexer());
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
	const auto pattern = m_language->GetLexer().GetPattern(index);
	if (pattern.IsEnding())
	{
		wxMessageBox(
			"You can't edit grammar's ending terminal.\n"
			"Please, choose another one.",
			"Information about terminal",
			wxOK | wxICON_INFORMATION);
		return;
	}

	TerminalEditDialog* editDialog = new TerminalEditDialog(pattern);
	if (editDialog->ShowModal() == wxID_OK)
	{
		wxComboBox* combo = editDialog->GetComboBox();
		wxTextCtrl* input = editDialog->GetTextCtrl();

		if (combo->IsEnabled())
		{
			const auto& predefined = GetPredefinedPatterns()[combo->GetSelection()];
			m_language->GetLexer().SetPattern(
				size_t(index),
				TokenPattern(pattern.GetName(), predefined.GetOrigin(),
					predefined.IsEnding(), predefined.GetPredefinedIndex())
			);
		}
		else if (input->IsEnabled())
		{
			m_language->GetLexer().SetPattern(
				size_t(index),
				TokenPattern(pattern.GetName(), input->GetValue(),
					pattern.IsEnding(), pattern.GetPredefinedIndex()));
		}
	}
	editDialog->Destroy();
}
