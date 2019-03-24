#include "pch.h"
#include "OutputView.h"

OutputView::OutputView(wxWindow* window)
	: wxPanel(window, wxID_ANY, wxDefaultPosition, wxSize(300, 150))
	, m_textCtrl(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE))
{
	m_textCtrl->SetEditable(false);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizer(sizer);
}

wxTextCtrl* OutputView::GetTextCtrl()
{
	return m_textCtrl;
}
