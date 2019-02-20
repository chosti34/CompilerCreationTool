#include "pch.h"
#include "TablePanel.h"
#include <wx/listctrl.h>
#include <map>

namespace
{
int GetBestColumnWidth(int index, int maxWidth)
{
	static const std::map<int, float> percentages = {
		{ 0, 0.2f },
		{ 1, 0.2f },
		{ 2, 0.2f },
		{ 3, 0.2f },
		{ 4, 0.2f }
	};

	auto found = percentages.find(index);
	assert(found != percentages.end());
	const float width = maxWidth * found->second;
	return int(width);
}
}

TablePanel::TablePanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(new wxListView(this, wxID_ANY))
{
	m_list->AppendColumn(wxT("Index"));
	m_list->AppendColumn(wxT("Name"));
	m_list->AppendColumn(wxT("Next"));
	m_list->AppendColumn(wxT("Flags"));
	m_list->AppendColumn(wxT("Acceptables"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_list, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit(sizer);
}

wxListView* TablePanel::GetListView()
{
	return m_list;
}

const wxListView* TablePanel::GetListView() const
{
	return m_list;
}

void TablePanel::AdjustColumnWidths()
{
	for (int i = 0; i < m_list->GetColumnCount(); ++i)
	{
		m_list->SetColumnWidth(i, GetBestColumnWidth(i, m_list->GetSize().GetWidth()));
	}
}
