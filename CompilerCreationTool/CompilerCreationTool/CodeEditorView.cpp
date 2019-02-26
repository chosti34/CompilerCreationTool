#include "pch.h"
#include "CodeEditorView.h"
#include <wx/statline.h>

namespace
{
wxStyledTextCtrl* SetupLeftPanel(wxPanel& panel)
{
	wxStaticText* title = new wxStaticText(&panel, wxID_ANY, wxT("Source code"));
	wxStaticLine* line = new wxStaticLine(&panel, wxID_ANY);

	wxStyledTextCtrl* input = new wxStyledTextCtrl(
		&panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	sizer->Add(line, 0, wxEXPAND | wxALL, 5);
	sizer->Add(input, 1, wxEXPAND | wxALL, 5);
	panel.SetSizer(sizer);

	return input;
}

wxStyledTextCtrl* SetupRightPanel(wxPanel& panel)
{
	wxStaticText* rightTitle = new wxStaticText(&panel, wxID_ANY, wxT("Output"));
	wxStaticLine* rightLine = new wxStaticLine(&panel, wxID_ANY);

	wxStyledTextCtrl* output = new wxStyledTextCtrl(
		&panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTE_DONTWRAP);
	output->SetEditable(false);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(rightTitle, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	sizer->Add(rightLine, 0, wxEXPAND | wxALL, 5);
	sizer->Add(output, 1, wxEXPAND | wxALL, 5);
	panel.SetSizer(sizer);

	return output;
}
}

CodeEditorView::CodeEditorView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	m_splitter = new wxSplitterWindow(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_LIVE_UPDATE | wxSP_3D);
	m_splitter->SetMinimumPaneSize(20);

	m_left = new wxPanel(m_splitter, wxID_ANY);
	m_input = SetupLeftPanel(*m_left);

	m_right = new wxPanel(m_splitter, wxID_ANY);
	m_output = SetupRightPanel(*m_right);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_splitter, 1, wxEXPAND);
	SetSizerAndFit(sizer);

	SetDoubleBuffered(true);
}

void CodeEditorView::SplitPanels(float sashPositionPercentage)
{
	assert(sashPositionPercentage <= 1.f);
	const int cWidth = m_splitter->GetSize().GetWidth();
	m_splitter->SplitVertically(m_left, m_right, sashPositionPercentage * cWidth);
}

wxString CodeEditorView::GetUserInput()
{
	return m_input->GetValue();
}
