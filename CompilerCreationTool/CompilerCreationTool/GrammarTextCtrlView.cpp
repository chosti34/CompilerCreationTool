#include "pch.h"
#include "GrammarTextCtrlView.h"

GrammarTextCtrlView::GrammarTextCtrlView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_textCtrl(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_TAB))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_textCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}
