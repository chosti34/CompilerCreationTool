#include "pch.h"
#include "TreeView.h"

namespace
{
const unsigned gcImageOffsetHor = 150;
const unsigned gcImageOffsetVert = 50;
const unsigned gcScrollUnits = 10;
}

class TreeView::ScrolledWindow : public wxScrolledWindow
{
public:
	TreeView::ScrolledWindow(wxWindow* parent)
		: wxScrolledWindow()
		, mBitmap(nullptr)
	{
		wxScrolledWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
		SetDoubleBuffered(true);
	}

	void SetImage(const wxImage& image)
	{
		mBitmap = std::make_unique<wxBitmap>(image);
		SetScrollbars(gcScrollUnits, gcScrollUnits, mBitmap->GetWidth(), mBitmap->GetHeight(), 0, 0, true);
		SetVirtualSize(mBitmap->GetWidth() + gcImageOffsetHor, mBitmap->GetHeight() + gcImageOffsetVert);
		Refresh(true);
	}

private:
	wxDECLARE_EVENT_TABLE();

	void OnPaint(wxPaintEvent&)
	{
		wxPaintDC dc(this);
		PrepareDC(dc);

		if (mBitmap)
		{
			dc.DrawBitmap(*mBitmap, gcImageOffsetHor, gcImageOffsetVert, true);
		}
	}

private:
	std::unique_ptr<wxBitmap> mBitmap;
};

wxBEGIN_EVENT_TABLE(TreeView::ScrolledWindow, wxScrolledWindow)
	EVT_PAINT(TreeView::ScrolledWindow::OnPaint)
wxEND_EVENT_TABLE()

TreeView::TreeView(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, mScrolledWindow(new ScrolledWindow(this))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(mScrolledWindow, 1, wxEXPAND | wxALL, 10);
	SetSizer(sizer);
	SetDoubleBuffered(true);
}

void TreeView::SetImage(const wxImage& image)
{
	assert(image.IsOk());
	mScrolledWindow->SetImage(image);
	Refresh(true);
}
