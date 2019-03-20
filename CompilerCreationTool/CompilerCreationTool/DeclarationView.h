#pragma once
#include "Signal.h"
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"
#include "../Parser/ParseResults.h"

#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <wx/notebook.h>

class DeclarationView : public wxPanel
{
public:
	using CursorUpdateSignal = Signal<void(int, int, int)>;
	using PositionChangeSignal = Signal<void(int, int)>;
	using ItemEditSignal = Signal<void(int)>;
	using SelectionSignal = Signal<void(int)>;

public:
	explicit DeclarationView(wxWindow* parent);
	wxString GetDeclaration() const;

	void SetLexerTerminals(const ILexer& lexer);
	void SetParserActions(const IParser<ParseResults>& parser);

	SignalScopedConnection DoOnTextCtrlCursorUpdate(CursorUpdateSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalPositionChange(PositionChangeSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalEdit(ItemEditSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalSelection(SelectionSignal::slot_type slot);
	SignalScopedConnection DoOnActionPositionChange(PositionChangeSignal::slot_type slot);
	SignalScopedConnection DoOnActionEdit(ItemEditSignal::slot_type slot);
	SignalScopedConnection DoOnActionSelection(SelectionSignal::slot_type slot);

private:
	void OnTerminalButtonUp();
	void OnTerminalButtonDown();
	void OnTerminalButtonEdit();

	void OnActionButtonUp();
	void OnActionButtonDown();
	void OnActionButtonEdit();

	void OnTerminalsListboxDoubleClick(wxCommandEvent& event);
	void OnTerminalsListboxMouseDown(wxMouseEvent& event);
	void OnTerminalsListboxSelection(wxCommandEvent& event);

	void OnActionsListboxDoubleClick(wxCommandEvent& event);
	void OnActionsListboxMouseDown(wxMouseEvent& event);
	void OnActionsListboxSelection(wxCommandEvent& event);

	void OnUpButton(wxCommandEvent& event);
	void OnDownButton(wxCommandEvent& event);
	void OnEditButton(wxCommandEvent& event);

	void OnTextCtrlCursorUpdate(wxStyledTextEvent& event);

private:
	wxStyledTextCtrl* m_input;

	wxListBox* m_terminalsListbox;
	wxListBox* m_actionsListbox;

	wxButton* mUpButton;
	wxButton* mDownButton;
	wxButton* mEditButton;

	// Signals
	CursorUpdateSignal mCursorUpdateSignal;
	PositionChangeSignal m_terminalPositionChangeSignal;
	SelectionSignal mTerminalSelectionSignal;
	ItemEditSignal m_terminalEditSignal;
	PositionChangeSignal m_actionPositionChangeSignal;
	SelectionSignal mActionSelectionSignal;
	ItemEditSignal m_actionEditSignal;
};
