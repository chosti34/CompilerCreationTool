#include "pch.h"
#include "TablePanel.h"
#include <wx/listctrl.h>

TablePanel::TablePanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_table(new wxListView(this, wxID_ANY))
{
	m_table->AppendColumn(wxT("Name"));
	m_table->AppendColumn(wxT("Column 2"));
	m_table->AppendColumn(wxT("Column 3"));
	m_table->AppendColumn(wxT("Column 4"));
	m_table->AppendColumn(wxT("Column 5"));
	m_table->AppendColumn(wxT("Column 6"));
	m_table->AppendColumn(wxT("Column 7"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_table, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}
