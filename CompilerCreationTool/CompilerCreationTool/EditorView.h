#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>

class EditorView : public wxPanel
{
public:
	// Line, column and character
	using CursorUpdateSignal = Signal<void(int, int, int)>;

	explicit EditorView(wxWindow* parent);
	SignalScopedConnection DoOnInputTextCtrlCursorUpdate(
		CursorUpdateSignal::slot_type slot);

	void SplitPanels(float sashPositionPercentage);
	wxStyledTextCtrl* GetOutputStyledTextCtrl();
	wxString GetUserInput();

private:
	void OnOutputStyledTextCtrlUpdateUI(wxStyledTextEvent& event);

private:
	CursorUpdateSignal mCursorUpdateSignal;
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxStyledTextCtrl* m_output;
	wxPanel* m_left;
	wxPanel* m_right;
};
