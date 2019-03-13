#pragma once

#include "Signal.h"
#include "../Lexer/ILexer.h"
#include "../Parser/IParser.h"

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
	void SetParserActions(const IParser<bool>& parser);
	void SplitPanels(float sashPositionPercentage);

	SignalScopedConnection DoOnTextCtrlCursorUpdate(CursorUpdateSignal::slot_type slot);

	SignalScopedConnection DoOnTerminalPositionChange(PositionChangeSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalEdit(ItemEditSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalSelection(SelectionSignal::slot_type slot);

	SignalScopedConnection DoOnActionPositionChange(PositionChangeSignal::slot_type slot);
	SignalScopedConnection DoOnActionEdit(ItemEditSignal::slot_type slot);
	SignalScopedConnection DoOnActionSelection(SelectionSignal::slot_type slot);

private:
	void CreateLeftPanelControls();
	void CreateRightPanelControls();
	wxPanel* CreateTerminalsPanel(wxNotebook* notebook);
	wxPanel* CreateActionsPanel(wxNotebook* notebook);

	// Event handlers
	void OnTerminalButtonUp(wxCommandEvent& event);
	void OnTerminalButtonDown(wxCommandEvent& event);
	void OnTerminalButtonEdit(wxCommandEvent& event);

	void OnTerminalsListboxDoubleClick(wxCommandEvent& event);
	void OnTerminalsListboxMouseDown(wxMouseEvent& event);
	void OnTerminalsListboxSelection(wxCommandEvent& event);

	void OnActionButtonUp(wxCommandEvent& event);
	void OnActionButtonDown(wxCommandEvent& event);
	void OnActionButtonEdit(wxCommandEvent& event);

	void OnActionsListboxDoubleClick(wxCommandEvent& event);
	void OnActionsListboxMouseDown(wxMouseEvent& event);
	void OnActionsListboxSelection(wxCommandEvent& event);

	void OnTextCtrlCursorUpdate(wxStyledTextEvent& event);

private:
	wxPanel* m_left;
	wxPanel* m_right;
	wxSplitterWindow* m_splitter;

	wxStyledTextCtrl* m_input;
	wxListBox* m_terminalsListbox;
	wxListBox* m_actionsListbox;
	wxButton* m_upTerminalButton;
	wxButton* m_downTerminalButton;
	wxButton* m_editTerminalButton;
	wxButton* m_upActionButton;
	wxButton* m_downActionButton;
	wxButton* m_editActionButton;

	CursorUpdateSignal mTextCtrlCursorUpdateSignal;

	PositionChangeSignal m_terminalPositionChangeSignal;
	SelectionSignal mTerminalSelectionSignal;
	ItemEditSignal m_terminalEditSignal;

	PositionChangeSignal m_actionPositionChangeSignal;
	SelectionSignal mActionSelectionSignal;
	ItemEditSignal m_actionEditSignal;
};
