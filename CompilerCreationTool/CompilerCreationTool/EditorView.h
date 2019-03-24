#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>

class EditorView : public wxPanel
{
public:
	using CursorUpdateSignal = Signal<void(int, int, int)>;

	explicit EditorView(wxWindow* parent);
	wxString GetUserInput() const;

	SignalScopedConnection DoOnInputTextCtrlCursorUpdate(CursorUpdateSignal::slot_type slot);

private:
	void OnUpdateUI(wxStyledTextEvent& event);

private:
	wxStyledTextCtrl* m_input;
	CursorUpdateSignal mCursorUpdateSignal;
};
