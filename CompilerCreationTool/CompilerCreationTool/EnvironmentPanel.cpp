#include "pch.h"
#include "EnvironmentPanel.h"
#include <wx/statbox.h>

EnvironmentPanel::EnvironmentPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_userInput(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxTE_MULTILINE))
	, m_intermediateCodeOutput(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxTE_MULTILINE))
{
	m_intermediateCodeOutput->SetEditable(false);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

	hSizer->Add(m_userInput, 1, wxEXPAND | wxALL, 5);
	hSizer->Add(m_intermediateCodeOutput, 1, wxEXPAND | wxALL, 5);

	SetSizerAndFit(hSizer);
}
