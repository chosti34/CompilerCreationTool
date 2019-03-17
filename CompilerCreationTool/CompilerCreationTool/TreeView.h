#pragma once
#include <wx/frame.h>
#include <wx/scrolwin.h>
#include <string>

class TreeView : public wxPanel
{
public:
	TreeView(wxWindow* parent);
	void SetImage(const wxImage& image);

private:
	class ScrolledWindow;
	ScrolledWindow* mScrolledWindow;
};
