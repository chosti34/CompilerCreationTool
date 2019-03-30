#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>

class EditorView : public wxPanel
{
public:
	explicit EditorView(wxWindow* parent);
	wxString GetUserInput() const;

	using UpdateUISignal = Signal<void(int, int, int)>;
	SignalScopedConnection DoOnTextCtrlUpdateUI(UpdateUISignal::slot_type slot);

	using FocusChangeSignal = Signal<void(bool)>;
	SignalScopedConnection DoOnFocusChange(FocusChangeSignal::slot_type slot);

private:
	void OnTextUpdateUI(wxStyledTextEvent& event);
	void OnTextFocusGain(wxFocusEvent& event);
	void OnTextFocusLost(wxFocusEvent& event);

private:
	wxStyledTextCtrl* mInput;
	UpdateUISignal mTextUpdateUISignal;
	FocusChangeSignal mFocusChangeSignal;
};
