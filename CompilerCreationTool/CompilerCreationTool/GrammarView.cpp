#include "pch.h"
#include "GrammarView.h"
#include "CtrlHelpers.h"
#include <wx/artprov.h>

GrammarView::GrammarView(wxWindow* parent)
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

	mInput->Bind(wxEVT_STC_UPDATEUI, &GrammarView::OnTextCtrlUpdateUI, this);
	mInput->Bind(wxEVT_SET_FOCUS, &GrammarView::OnTextFocusGain, this);
	mInput->Bind(wxEVT_KILL_FOCUS, &GrammarView::OnTextFocusLost, this);
}

wxString GrammarView::GetDeclaration() const
{
	return mInput->GetValue();
}

SignalScopedConnection GrammarView::DoOnTextCtrlUpdateUI(UpdateUISignal::slot_type slot)
{
	return mTextUpdateUISignal.connect(slot);
}

SignalScopedConnection GrammarView::DoOnFocusChange(FocusChangeSignal::slot_type slot)
{
	return mFocusChangeSignal.connect(slot);
}

void GrammarView::OnTextCtrlUpdateUI(wxStyledTextEvent& event)
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

void GrammarView::OnTextFocusGain(wxFocusEvent& event)
{
	std::cout << "Grammar focus gain\n";
	mFocusChangeSignal(true);
	event.Skip(true);
}

void GrammarView::OnTextFocusLost(wxFocusEvent& event)
{
	std::cout << "Grammar focus lost\n";
	mFocusChangeSignal(false);
	event.Skip(true);
}
