#include "pch.h"
#include "EnvironmentPanel.h"
#include <wx/statline.h>

EnvironmentPanel::EnvironmentPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_splitter = new wxSplitterWindow(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_LIVE_UPDATE | wxSP_3D);

	m_left = new wxPanel(m_splitter, wxID_ANY);
	m_right = new wxPanel(m_splitter, wxID_ANY);

	// Setting up left panel
	wxStaticText* leftTitle = new wxStaticText(m_left, wxID_ANY,
		wxT("Source code"));
	wxStaticLine* leftLine = new wxStaticLine(m_left, wxID_ANY);
	m_input = new wxStyledTextCtrl(m_left, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);

	wxBoxSizer* leftBoxSizer = new wxBoxSizer(wxVERTICAL);
	leftBoxSizer->Add(leftTitle, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	leftBoxSizer->Add(leftLine, 0, wxEXPAND | wxALL, 5);
	leftBoxSizer->Add(m_input, 1, wxEXPAND | wxALL, 5);
	m_left->SetSizer(leftBoxSizer);

	// Setting up right panel
	wxStaticText* rightTitle = new wxStaticText(m_right, wxID_ANY,
		wxT("Output"));
	wxStaticLine* rightLine = new wxStaticLine(m_right, wxID_ANY);
	m_output = new wxStyledTextCtrl(m_right, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	m_output->SetEditable(false);

	wxBoxSizer* rightBoxSizer = new wxBoxSizer(wxVERTICAL);
	rightBoxSizer->Add(rightTitle, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	rightBoxSizer->Add(rightLine, 0, wxEXPAND | wxALL, 5);
	rightBoxSizer->Add(m_output, 1, wxEXPAND | wxALL, 5);
	m_right->SetSizer(rightBoxSizer);

	// Setting up main sizer
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(m_splitter, 1, wxEXPAND);
	SetSizerAndFit(hSizer);

	// To avoid flickering
	SetDoubleBuffered(true);
}

wxStyledTextCtrl* EnvironmentPanel::GetInputControl()
{
	return m_input;
}

wxStyledTextCtrl* EnvironmentPanel::GetOutputControl()
{
	return m_output;
}

void EnvironmentPanel::Split()
{
	m_splitter->SplitVertically(m_left, m_right);
	m_splitter->SetMinimumPaneSize(20);
}

wxBEGIN_EVENT_TABLE(EnvironmentPanel, wxPanel)
wxEND_EVENT_TABLE()
