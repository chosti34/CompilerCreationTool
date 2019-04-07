#pragma once
#include "Signal.h"
#include <wx/panel.h>
#include <wx/stc/stc.h>

class TextView : public wxPanel
{
public:
	explicit TextView(wxWindow* parent);
	wxString GetText() const;

	int GetCurrentLine() const;
	int GetCurrentCol() const;
	int GetCurrentCh() const;

	using UpdateUISignal = Signal<void(int, int, int)>;
	SignalScopedConnection DoOnUIUpdate(UpdateUISignal::slot_type slot);

	using FocusChangeSignal = Signal<void(bool)>;
	SignalScopedConnection DoOnFocusChange(FocusChangeSignal::slot_type slot);

private:
	void OnUpdateUI(wxStyledTextEvent& event);
	void OnTextFocusGain(wxFocusEvent& event);
	void OnTextFocusLost(wxFocusEvent& event);

private:
	wxStyledTextCtrl* mInput;
	UpdateUISignal mUpdateUISignal;
	FocusChangeSignal mFocusChangeSignal;
};
