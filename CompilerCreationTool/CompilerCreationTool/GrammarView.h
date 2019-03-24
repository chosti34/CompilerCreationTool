#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>

class GrammarView : public wxPanel
{
public:
	explicit GrammarView(wxWindow* parent);
	wxString GetDeclaration() const;

	SignalScopedConnection DoOnTextCtrlUpdateUI(Signal<void(int, int, int)>::slot_type slot);

private:
	void OnTextCtrlUpdateUI(wxStyledTextEvent& event);

private:
	wxStyledTextCtrl* m_input;
	Signal<void(int, int, int)> m_updateUISignal;
};
