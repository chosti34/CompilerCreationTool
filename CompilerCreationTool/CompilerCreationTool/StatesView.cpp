#include "pch.h"
#include "StatesView.h"
#include "../Utils/string_utils.h"

namespace
{
const std::string gcNoItem = "<none>";
const int gcBorderSize = 3;

std::string GetStateFlagsRepresentation(const IParserState& state)
{
	std::set<std::string> flags;
	for (const StateFlag& flag : GetAllStateFlags())
	{
		if (state.GetFlag(flag))
		{
			flags.insert(ToString(flag));
		}
	}
	return flags.empty() ? gcNoItem : string_utils::JoinStrings(flags);
};
}

StatesView::StatesView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(new wxListView(this, wxID_ANY))
{
	m_list->AppendColumn(wxT("Index"));
	m_list->AppendColumn(wxT("Name"));
	m_list->AppendColumn(wxT("Next"));
	m_list->AppendColumn(wxT("Flags"));
	m_list->AppendColumn(wxT("Acceptables"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_list, 1, wxEXPAND | wxALL, gcBorderSize);
	SetSizer(sizer);

	Bind(wxEVT_SIZE, &StatesView::OnListResize, this);
}

void StatesView::SetParserTable(const IParserTable& table)
{
	m_list->DeleteAllItems();

	for (size_t i = 0; i < table.GetStatesCount(); ++i)
	{
		const long index = m_list->InsertItem(m_list->GetItemCount(), std::to_string(i));

		const IParserState& state = table.GetState(i);
		const std::string& name = state.GetName();
		const boost::optional<size_t> address = state.GetNextAddress();
		const bool isAttribute = state.GetFlag(StateFlag::Attribute);
		const std::set<std::string>& acceptables = state.GetAcceptableTerminals();

		m_list->SetItem(index, 1, name);
		m_list->SetItem(index, 2, address ? std::to_string(*address) : gcNoItem);
		m_list->SetItem(index, 3, GetStateFlagsRepresentation(state));
		m_list->SetItem(index, 4, isAttribute ? gcNoItem : string_utils::JoinStrings(acceptables));
	}
}

void StatesView::ClearItems()
{
	m_list->DeleteAllItems();
}

void StatesView::AdjustColumnWidth(int width)
{
	m_list->Freeze();

	for (int i = 0; i < m_list->GetColumnCount(); ++i)
	{
		assert(m_list->GetColumnCount() != 0);
		const float cCoeff = 1.f / m_list->GetColumnCount();
		m_list->SetColumnWidth(i, cCoeff * (width - gcBorderSize * 2));
	}

	m_list->Thaw();
}

void StatesView::OnListResize(wxSizeEvent& event)
{
	AdjustColumnWidth(event.GetSize().x);
	event.Skip(true);
}
