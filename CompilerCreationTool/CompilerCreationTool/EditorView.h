#pragma once
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

class EditorView : public wxPanel
{
public:
	explicit EditorView(wxWindow* parent);

	void SplitPanels(float sashPositionPercentage);
	wxString GetUserInput();

	wxStyledTextCtrl* GetOutputStyledTextCtrl();

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxStyledTextCtrl* m_output;
	wxPanel* m_left;
	wxPanel* m_right;
};
