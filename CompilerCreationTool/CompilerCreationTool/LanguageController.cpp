#include "pch.h"
#include "LanguageController.h"
#include "TerminalEditDialog.h"
#include "ActionEditDialog.h"
#include "TextCtrlLogger.h"
#include "LanguageSerialization.h"
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

void ClearStatusBarTextFields(wxStatusBar& statusbar, std::vector<int> && fields)
{
	for (auto field : fields)
	{
		statusbar.SetStatusText(wxEmptyString, field);
	}
}
}

LanguageController::LanguageController(Language* language, MainFrame* frame)
	: mLanguage(language)
	, mFrame(frame)
	, mGrammarView(mFrame->GetDeclarationView())
	, mStatesView(mFrame->GetStatesView())
	, mEditorView(mFrame->GetEditorView())
	, mTreeView(mFrame->GetTreeView())
	, mTerminalsView(mFrame->GetTerminalsView())
	, mActionsView(mFrame->GetActionsView())
	, mOutputView(mFrame->GetOutputView())
{
	namespace ph = std::placeholders;

	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::New, std::bind(&LanguageController::OnNewButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Open, std::bind(&LanguageController::OnOpenButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Save, std::bind(&LanguageController::OnSaveButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::SaveAs, std::bind(&LanguageController::OnSaveAsButtonPress, this)));

	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Build, std::bind(&LanguageController::OnBuildButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Run, std::bind(&LanguageController::OnRunButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Info, std::bind(&LanguageController::OnInfoButtonPress, this)));

	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Up, std::bind(&LanguageController::OnUpButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Down, std::bind(&LanguageController::OnDownButtonPress, this)));
	mConnections.push_back(mFrame->DoOnButtonPress(Buttons::Edit, std::bind(&LanguageController::OnEditButtonPress, this)));

	mConnections.push_back(mTerminalsView->DoOnItemDeselection(std::bind(&LanguageController::OnTerminalsViewDeselection, this)));
	mConnections.push_back(mActionsView->DoOnItemDeselection(std::bind(&LanguageController::OnActionsViewDeselection, this)));

	mConnections.push_back(mTerminalsView->DoOnItemSelection(std::bind(&LanguageController::OnTerminalSelection, this, ph::_1)));
	mConnections.push_back(mActionsView->DoOnItemSelection(std::bind(&LanguageController::OnActionSelection, this, ph::_1)));

	mConnections.push_back(mTerminalsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnTerminalEdit, this, ph::_1)));
	mConnections.push_back(mActionsView->DoOnItemDoubleSelection(std::bind(&LanguageController::OnActionEdit, this, ph::_1)));

	mConnections.push_back(mGrammarView->DoOnUIUpdate(std::bind(&LanguageController::OnGrammarTextCtrlUpdateUI, this, ph::_1, ph::_2, ph::_3)));
	mConnections.push_back(mEditorView->DoOnUIUpdate(std::bind(&LanguageController::OnEditorTextCtrlUpdateUI, this, ph::_1, ph::_2, ph::_3)));

	mConnections.push_back(mGrammarView->DoOnFocusChange(std::bind(&LanguageController::OnGrammarTextFocusChange, this, ph::_1)));
	mConnections.push_back(mEditorView->DoOnFocusChange(std::bind(&LanguageController::OnEditorTextFocusChange, this, ph::_1)));
}

void LanguageController::UpdateStatusBarOnTextFocusChange(TextView& view, const std::string& name, bool focus)
{
	wxStatusBar* statusbar = mFrame->GetStatusBar();

	if (focus)
	{
		UpdateStatusBarCursorInfo(view.GetCurrentLine(), view.GetCurrentCol(), view.GetCurrentCh());
		statusbar->PushStatusText(name, StatusBarFields::ContextInfo);
	}
	else
	{
		ClearStatusBarTextFields(*statusbar, { StatusBarFields::Line, StatusBarFields::Column, StatusBarFields::Ch });
		statusbar->PopStatusText(4);
	}
}

void LanguageController::UpdateStatusBarCursorInfo(int line, int col, int ch)
{
	wxStatusBar* statusbar = mFrame->GetStatusBar();
	statusbar->SetStatusText("Ln " + std::to_string(line), StatusBarFields::Line);
	statusbar->SetStatusText("Col " + std::to_string(col), StatusBarFields::Column);
	statusbar->SetStatusText("Ch " + std::to_string(ch), StatusBarFields::Ch);
}

void LanguageController::SwapTerminalPositions(int from, int to)
{
	assert(mLanguage->IsInitialized());
	mLanguage->GetLexer().SwapPatterns(size_t(from), size_t(to));
}

void LanguageController::SwapActionPositions(int from, int to)
{
	assert(mLanguage->IsInitialized());
	mLanguage->GetParser().SwapActions(size_t(from), size_t(to));
}

void LanguageController::OnNewButtonPress()
{
	std::cout << "new\n";
}

void LanguageController::OnOpenButtonPress()
{
	UnserializeLanguage("saved.xml", *mLanguage);
}

void LanguageController::OnSaveButtonPress()
{
	SerializeLanguage("saved.xml", *mLanguage);
}

void LanguageController::OnSaveAsButtonPress()
{
	std::cout << "save as\n";
}

void LanguageController::OnBuildButtonPress()
{
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	mLanguage->SetGrammar(builder->CreateGrammar(mGrammarView->GetText()));
	mLanguage->GetParser().SetLogger(std::make_unique<TextCtrlLogger>(mOutputView->GetTextCtrl()));
	mTerminalsView->SetItems(GetTerminalsArray(mLanguage->GetLexer()));
	mActionsView->SetItems(GetActionsArray(mLanguage->GetParser()));
	mStatesView->SetParserTable(mLanguage->GetParser().GetTable());
	mFrame->GetToolBar()->EnableTool(Buttons::Run, true);
	mFrame->GetToolBar()->EnableTool(Buttons::Info, true);
}

void LanguageController::OnRunButtonPress()
{
	assert(mLanguage->IsInitialized());

	IParser<ParseResults>& parser = mLanguage->GetParser();
	IParserLogger* logger = parser.GetLogger();
	assert(logger);

	logger->Log("[" + time_utils::GetCurrentTimeAsString() + "] Parsing started...\n");
	const ParseResults results = parser.Parse(mEditorView->GetText().ToStdString());
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
				mTreeView->SetImage(img);
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
		mTreeView->UnsetImage();
	}

	logger->Log("=========================\n");
}

void LanguageController::OnInfoButtonPress()
{
	assert(mLanguage->IsInitialized());
	LanguageInformationDialog dialog(mFrame, mLanguage->GetInfo());
	dialog.ShowModal();
}

void LanguageController::OnUpButtonPress()
{
	if (mTerminalsView->HasSelection() && mTerminalsView->MoveSelectionUp())
	{
		SwapTerminalPositions(mTerminalsView->GetSelection(), mTerminalsView->GetSelection() + 1);
	}
	else if (mActionsView->HasSelection() && mActionsView->MoveSelectionUp())
	{
		SwapActionPositions(mActionsView->GetSelection(), mActionsView->GetSelection() + 1);
	}
}

void LanguageController::OnDownButtonPress()
{
	if (mTerminalsView->HasSelection() && mTerminalsView->MoveSelectionDown())
	{
		SwapTerminalPositions(mTerminalsView->GetSelection(), mTerminalsView->GetSelection() - 1);
	}
	else if (mActionsView->HasSelection() && mActionsView->MoveSelectionDown())
	{
		SwapActionPositions(mActionsView->GetSelection(), mActionsView->GetSelection() - 1);
	}
}

void LanguageController::OnEditButtonPress()
{
	if (mTerminalsView->HasSelection())
	{
		OnTerminalEdit(mTerminalsView->GetSelection());
	}
	else if (mActionsView->HasSelection())
	{
		OnActionEdit(mActionsView->GetSelection());
	}
}

void LanguageController::OnTerminalsViewDeselection()
{
	std::cout << "Term deselection" << std::endl;
	mFrame->GetToolBar()->EnableTool(Buttons::Up, false);
	mFrame->GetToolBar()->EnableTool(Buttons::Down, false);
	mFrame->GetToolBar()->EnableTool(Buttons::Edit, false);
	mActionsView->DeselectAll();
}

void LanguageController::OnActionsViewDeselection()
{
	std::cout << "Action deselection" << std::endl;
	mFrame->GetToolBar()->EnableTool(Buttons::Up, false);
	mFrame->GetToolBar()->EnableTool(Buttons::Down, false);
	mFrame->GetToolBar()->EnableTool(Buttons::Edit, false);
	mTerminalsView->DeselectAll();
}

void LanguageController::OnTerminalSelection(int)
{
	mActionsView->DeselectAll();
	mFrame->GetToolBar()->EnableTool(Buttons::Up, true);
	mFrame->GetToolBar()->EnableTool(Buttons::Down, true);
	mFrame->GetToolBar()->EnableTool(Buttons::Edit, true);
}

void LanguageController::OnActionSelection(int)
{
	mTerminalsView->DeselectAll();
	mFrame->GetToolBar()->EnableTool(Buttons::Up, true);
	mFrame->GetToolBar()->EnableTool(Buttons::Down, true);
	mFrame->GetToolBar()->EnableTool(Buttons::Edit, true);
}

void LanguageController::OnTerminalEdit(int index)
{
	assert(mLanguage->IsInitialized());
	assert(index < mLanguage->GetLexer().GetPatternsCount());

	TokenPattern& pattern = mLanguage->GetLexer().GetPattern(index);
	if (pattern.IsEnding())
	{
		using namespace std::string_literals;
		const wxString cTitle = wxT("Information About Terminal '" + pattern.GetName() + "'");
		const wxString cMessage = "'"s + pattern.GetName() +
			"' is grammar's ending terminal - you cannot edit it."s;
		wxMessageBox(cMessage, cTitle, wxOK | wxICON_WARNING);
		return;
	}

	TerminalEditDialog dialog(mFrame, pattern);
	dialog.ShowModal();

	OnTerminalSelection(index);
}

void LanguageController::OnActionEdit(int index)
{
	assert(mLanguage->IsInitialized());
	assert(index < mLanguage->GetParser().GetActionsCount());

	IAction& action = mLanguage->GetParser().GetAction(index);
	ActionEditDialog dialog(mFrame, action);

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
	UpdateStatusBarCursorInfo(line, col, ch);
}

void LanguageController::OnEditorTextCtrlUpdateUI(int line, int col, int ch)
{
	UpdateStatusBarCursorInfo(line, col, ch);
}

void LanguageController::OnGrammarTextFocusChange(bool focus)
{
	UpdateStatusBarOnTextFocusChange(*mGrammarView, "Grammar", focus);
}

void LanguageController::OnEditorTextFocusChange(bool focus)
{
	UpdateStatusBarOnTextFocusChange(*mEditorView, "Editor", focus);
}
