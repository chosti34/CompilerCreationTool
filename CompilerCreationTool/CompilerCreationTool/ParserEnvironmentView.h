#pragma once
#include <wx/panel.h>
#include <wx/window.h>

class ParserEnvironmentView : public wxPanel
{
public:
	ParserEnvironmentView(wxWindow* parent);

private:
	wxStaticBox* m_box;
};
