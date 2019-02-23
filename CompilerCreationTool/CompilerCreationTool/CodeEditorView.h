#pragma once
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

class CodeEditorView : public wxPanel
{
public:
	CodeEditorView(wxWindow* parent);

	void SplitPanels(float sashPositionPercentage);
	wxString GetUserInput();

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxStyledTextCtrl* m_output;
	wxPanel* m_left;
	wxPanel* m_right;
};
