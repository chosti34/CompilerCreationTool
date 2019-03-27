#include "pch.h"
#include "GrammarView.h"
#include "CtrlHelpers.h"
#include <wx/artprov.h>

GrammarView::GrammarView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_input = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP);

	m_input->SetScrollWidth(m_input->GetSize().GetWidth());
	m_input->SetScrollWidthTracking(true);
	SetupStyledTextCtrlMargins(*m_input);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_input, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);

	m_input->Bind(wxEVT_STC_UPDATEUI, &GrammarView::OnTextCtrlUpdateUI, this);
}

wxString GrammarView::GetDeclaration() const
{
	return m_input->GetValue();
}

SignalScopedConnection GrammarView::DoOnTextCtrlUpdateUI(Signal<void(int, int, int)>::slot_type slot)
{
	return m_updateUISignal.connect(slot);
}

void GrammarView::OnTextCtrlUpdateUI(wxStyledTextEvent& event)
{
	int linePos = 0;
	m_input->GetCurLine(&linePos);

	m_updateUISignal(
		m_input->GetCurrentLine() + 1,
		m_input->GetColumn(m_input->GetCurrentPos()) + 1,
		linePos + 1
	);

	event.Skip(true);
}
