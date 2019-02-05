#include "pch.h"
#include "EnvironmentPanel.h"
#include <wx/statbox.h>

EnvironmentPanel::EnvironmentPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_box(new wxStaticBox(this, wxID_ANY, wxT("Label")))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_box, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}
