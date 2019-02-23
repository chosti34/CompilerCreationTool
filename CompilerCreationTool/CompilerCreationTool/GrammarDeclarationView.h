#pragma once
#include "../Lexer/ILexer.h"
#include "Signal.h"
#include <string>
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>

class GrammarDeclarationView : public wxPanel
{
public:
	using PositionChangeSignal = Signal<void(int, int)>;

public:
	explicit GrammarDeclarationView(wxWindow* parent);

	SignalScopedConnection DoOnTerminalPositionChange(
		PositionChangeSignal::slot_type slot);

	wxString GetDeclaration() const;
	void SetLexerTerminals(const ILexer& lexer);
	void SplitPanels(float sashPositionPercentage);

private:
	void OnTerminalButtonUp(wxCommandEvent&);
	void OnTerminalButtonDown(wxCommandEvent&);
	void OnTerminalButtonEdit(wxCommandEvent&);

private:
	wxSplitterWindow* mSplitter;
	wxStyledTextCtrl* mTextControl;
	wxPanel* mLeftTextControlPanel;
	wxPanel* mRightNotebookPanel;
	wxListBox* mTerminalsListbox;
	wxListBox* mActionsListbox;
	wxButton* mUpTerminalButton;
	wxButton* mDownTerminalButton;
	wxButton* mEditTerminalButton;

	PositionChangeSignal m_terminalPositionChangeSignal;
};
