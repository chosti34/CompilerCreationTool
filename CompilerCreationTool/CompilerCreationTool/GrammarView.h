#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>

class GrammarView : public wxPanel
{
public:
	explicit GrammarView(wxWindow* parent);
	wxString GetDeclaration() const;

	using UpdateUISignal = Signal<void(int, int, int)>;
	SignalScopedConnection DoOnTextCtrlUpdateUI(Signal<void(int, int, int)>::slot_type slot);

	using FocusChangeSignal = Signal<void(bool)>;
	SignalScopedConnection DoOnFocusChange(FocusChangeSignal::slot_type slot);

private:
	void OnTextCtrlUpdateUI(wxStyledTextEvent& event);
	void OnTextFocusGain(wxFocusEvent& event);
	void OnTextFocusLost(wxFocusEvent& event);

private:
	wxStyledTextCtrl* mInput;
	UpdateUISignal mTextUpdateUISignal;
	FocusChangeSignal mFocusChangeSignal;
};
