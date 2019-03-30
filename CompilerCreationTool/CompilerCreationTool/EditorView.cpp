#include "pch.h"
#include "EditorView.h"
#include "CtrlHelpers.h"
#include <wx/statline.h>

EditorView::EditorView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	mInput = new wxStyledTextCtrl(
		this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	SetupStyledTextCtrlMargins(*mInput);

	mInput->SetScrollWidth(mInput->GetSize().GetWidth());
	mInput->SetScrollWidthTracking(true);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(mInput, 1, wxEXPAND);
	SetSizerAndFit(sizer);

	mInput->Bind(wxEVT_STC_UPDATEUI, &EditorView::OnTextUpdateUI, this);
	mInput->Bind(wxEVT_SET_FOCUS, &EditorView::OnTextFocusGain, this);
	mInput->Bind(wxEVT_KILL_FOCUS, &EditorView::OnTextFocusLost, this);
}

SignalScopedConnection EditorView::DoOnTextCtrlUpdateUI(UpdateUISignal::slot_type slot)
{
	return mTextUpdateUISignal.connect(slot);
}

SignalScopedConnection EditorView::DoOnFocusChange(FocusChangeSignal::slot_type slot)
{
	return mFocusChangeSignal.connect(slot);
}

wxString EditorView::GetUserInput() const
{
	return mInput->GetValue();
}

void EditorView::OnTextUpdateUI(wxStyledTextEvent& event)
{
	int linePos = 0;
	mInput->GetCurLine(&linePos);

	mTextUpdateUISignal(
		mInput->GetCurrentLine() + 1,
		mInput->GetColumn(mInput->GetCurrentPos()) + 1,
		linePos + 1
	);

	event.Skip(true);
}

void EditorView::OnTextFocusGain(wxFocusEvent& event)
{
	std::cout << "Editor focus gain\n";
	mFocusChangeSignal(true);
	event.Skip(true);
}

void EditorView::OnTextFocusLost(wxFocusEvent& event)
{
	std::cout << "Editor focus lost\n";
	mFocusChangeSignal(false);
	event.Skip(true);
}
