#include "pch.h"
#include "EntitiesListboxView.h"

namespace
{
const unsigned gcBorderSize = 3;
}

EntitiesListboxView::EntitiesListboxView(wxWindow* parent, const std::string& leftColumn, const std::string& rightColumn)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 120))
	, mLeftColumn(leftColumn)
	, mRightColumn(rightColumn)
{
	mListCtrl = new wxListCtrl(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxLC_REPORT | wxLC_SINGLE_SEL | wxVSCROLL | wxHSCROLL);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(mListCtrl, 1, wxEXPAND | wxALL, gcBorderSize);
	SetSizer(sizer);

	mListCtrl->InsertColumn(0, mLeftColumn);
	mListCtrl->InsertColumn(1, mRightColumn);

	Bind(wxEVT_SIZE, &EntitiesListboxView::OnResize, this);
	mListCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, &EntitiesListboxView::OnListCtrlItemSelection, this);
	mListCtrl->Bind(wxEVT_LIST_ITEM_ACTIVATED, &EntitiesListboxView::OnListCtrlItemDoubleSelection, this);
	mListCtrl->Bind(wxEVT_LEFT_DOWN, &EntitiesListboxView::OnListCtrlMouseDown, this);
}

void EntitiesListboxView::SetItems(const std::vector<std::pair<std::string, std::string>> &items)
{
	mListCtrl->DeleteAllItems();
	for (const auto& pair : items)
	{
		const long index = mListCtrl->InsertItem(mListCtrl->GetItemCount(), pair.first);
		mListCtrl->SetItem(index, 1, pair.second);
	}

	AdjustColumnWidth(mListCtrl->GetClientSize().x);
}

void EntitiesListboxView::SetItemValue(int index, const wxString& value)
{
	if (index >= mListCtrl->GetItemCount())
	{
		throw std::out_of_range("index must be less than items count");
	}
	mListCtrl->SetItem(index, 1, value);
}

void EntitiesListboxView::ClearItems()
{
	mListCtrl->DeleteAllItems();
}

void EntitiesListboxView::DeselectAll()
{
	for (int i = 0; i < mListCtrl->GetItemCount(); ++i)
	{
		mListCtrl->SetItemState(i, 0, wxLIST_STATE_SELECTED);
	}
}

int EntitiesListboxView::GetSelection() const
{
	return mListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

bool EntitiesListboxView::HasSelection() const
{
	return GetSelection() != wxNOT_FOUND;
}

bool EntitiesListboxView::MoveSelectionUp()
{
	const int selection = GetSelection();
	assert(selection != wxNOT_FOUND);
	const int upper = selection - 1;

	if (upper >= 0)
	{
		// Swap values
		std::pair<wxString, wxString> swapValue;
		swapValue.first = mListCtrl->GetItemText(selection, 0);
		swapValue.second = mListCtrl->GetItemText(selection, 1);

		mListCtrl->SetItem(selection, 0, mListCtrl->GetItemText(upper, 0));
		mListCtrl->SetItem(selection, 1, mListCtrl->GetItemText(upper, 1));

		mListCtrl->SetItem(upper, 0, swapValue.first);
		mListCtrl->SetItem(upper, 1, swapValue.second);

		// Select upper item
		mListCtrl->SetItemState(upper, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

		return true;
	}

	return false;
}

bool EntitiesListboxView::MoveSelectionDown()
{
	const int selection = GetSelection();
	assert(selection != wxNOT_FOUND);
	const int lower = selection + 1;

	if (lower < mListCtrl->GetItemCount())
	{
		// Swap values
		std::pair<wxString, wxString> swapValue;
		swapValue.first = mListCtrl->GetItemText(selection, 0);
		swapValue.second = mListCtrl->GetItemText(selection, 1);

		mListCtrl->SetItem(selection, 0, mListCtrl->GetItemText(lower, 0));
		mListCtrl->SetItem(selection, 1, mListCtrl->GetItemText(lower, 1));

		mListCtrl->SetItem(lower, 0, swapValue.first);
		mListCtrl->SetItem(lower, 1, swapValue.second);

		// Select upper item
		mListCtrl->SetItemState(lower, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

		return true;
	}

	return false;
}

SignalScopedConnection EntitiesListboxView::DoOnItemDoubleSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemDoubleSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnItemSelection(Signal<void(int)>::slot_type slot)
{
	return m_itemSelectionSignal.connect(slot);
}

SignalScopedConnection EntitiesListboxView::DoOnItemDeselection(Signal<void()>::slot_type slot)
{
	return m_itemDeselectionSignal.connect(slot);
}

void EntitiesListboxView::AdjustColumnWidth(int width)
{
	mListCtrl->Freeze();

	for (int i = 0; i < mListCtrl->GetColumnCount(); ++i)
	{
		assert(mListCtrl->GetColumnCount() != 0);
		const float cCoeff = 1.f / mListCtrl->GetColumnCount();
		mListCtrl->SetColumnWidth(i, cCoeff * width);
	}

	mListCtrl->Thaw();
	Refresh(true);
}

void EntitiesListboxView::OnResize(wxSizeEvent& event)
{
	if (mListCtrl->GetItemCount() != 0)
	{
		int width;
		mListCtrl->GetClientSize(&width, nullptr);
		AdjustColumnWidth(width);
	}
	else
	{
		AdjustColumnWidth(mListCtrl->GetSize().x);
	}
	event.Skip(true);
}

void EntitiesListboxView::OnListCtrlMouseDown(wxMouseEvent& event)
{
	// ≈сли пользователь кликнул по пустой части списка, тогда полностью снимаем все выделени€
	wxArrayInt selections;
	int flags;
	if (mListCtrl->HitTest(event.GetPosition(), flags) == wxNOT_FOUND)
	{
		DeselectAll();
		m_itemDeselectionSignal();
	}
	event.Skip(true);
}

void EntitiesListboxView::OnListCtrlItemSelection(wxListEvent&)
{
	const int selection = GetSelection();
	assert(selection != wxNOT_FOUND);
	m_itemSelectionSignal(selection);
}

void EntitiesListboxView::OnListCtrlItemDoubleSelection(wxListEvent&)
{
	const int selection = GetSelection();
	assert(selection != wxNOT_FOUND);
	m_itemDoubleSelectionSignal(selection);
}
