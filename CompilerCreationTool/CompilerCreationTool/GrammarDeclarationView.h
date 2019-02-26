#pragma once
#include "Signal.h"
#include "../Lexer/ILexer.h"
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <wx/notebook.h>

class GrammarDeclarationView : public wxPanel
{
public:
	explicit GrammarDeclarationView(wxWindow* parent);

	wxString GetDeclaration() const;
	void SetLexerTerminals(const ILexer& lexer);
	void SplitPanels(float sashPositionPercentage);

	using PositionChangeSignal = Signal<void(int, int)>;
	using TerminalEditSignal = Signal<void(int, const std::string&)>;
	using GetTerminalPatternSignal = Signal<const TokenPattern&(int)>;

	SignalScopedConnection DoOnTerminalPositionChange(PositionChangeSignal::slot_type slot);
	SignalScopedConnection DoOnTerminalEdit(TerminalEditSignal::slot_type slot);
	SignalScopedConnection DoOnGetTerminalPattern(GetTerminalPatternSignal::slot_type slot);

private:
	void CreateLeftPanelControls();
	void CreateRightPanelControls();
	wxPanel* CreateTerminalsPanel(wxNotebook*);
	wxPanel* CreateActionsPanel(wxNotebook*);

	void OnTerminalButtonUp(wxCommandEvent&);
	void OnTerminalButtonDown(wxCommandEvent&);
	void OnTerminalButtonEdit(wxCommandEvent&);

	void OnTerminalsListboxDoubleClick(wxCommandEvent&);
	void OnTerminalsListboxMouseDown(wxMouseEvent&);

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxPanel* m_left;
	wxPanel* m_right;
	wxListBox* m_terminalsListbox;
	wxListBox* m_actionsListbox;
	wxButton* m_upTerminalButton;
	wxButton* m_downTerminalButton;
	wxButton* m_editTerminalButton;
	wxButton* m_upActionButton;
	wxButton* m_downActionButton;
	wxButton* m_editActionButton;
	PositionChangeSignal m_terminalPositionChangeSignal;
	TerminalEditSignal m_terminalEditSignal;
	GetTerminalPatternSignal m_getTerminalPatternSignal;
};
