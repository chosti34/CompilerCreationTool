#include "pch.h"
#include "EditorView.h"
#include "CtrlHelpers.h"
#include <wx/statline.h>

namespace
{
wxStyledTextCtrl* SetupLeftPanel(wxPanel& panel)
{
	wxStyledTextCtrl* input = new wxStyledTextCtrl(
		&panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	SetupStyledTextCtrlMargins(*input);

	input->SetScrollWidth(input->GetSize().GetWidth());
	input->SetScrollWidthTracking(true);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(input, 1, wxEXPAND | wxALL, 5);
	panel.SetSizer(sizer);

	return input;
}
}

EditorView::EditorView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	m_input = SetupLeftPanel(*panel);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(sizer);

	m_input->Bind(wxEVT_STC_UPDATEUI, &EditorView::OnUpdateUI, this);
}

SignalScopedConnection EditorView::DoOnTextCtrlUpdateUI(
	CursorUpdateSignal::slot_type slot)
{
	return mUpdateUISignal.connect(slot);
}

wxString EditorView::GetUserInput() const
{
	return m_input->GetValue();
}

void EditorView::OnUpdateUI(wxStyledTextEvent& event)
{
	int linePos = 0;
	m_input->GetCurLine(&linePos);

	mUpdateUISignal(
		m_input->GetCurrentLine() + 1,
		m_input->GetColumn(m_input->GetCurrentPos()) + 1,
		linePos + 1
	);

	event.Skip(true);
}
