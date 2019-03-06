#pragma once
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

class EditorView : public wxPanel
{
public:
	EditorView(wxWindow* parent);

	void SplitPanels(float sashPositionPercentage);
	wxString GetUserInput();

	void LogOutput(const std::string& message);

private:
	wxSplitterWindow* m_splitter;
	wxStyledTextCtrl* m_input;
	wxStyledTextCtrl* m_output;
	wxPanel* m_left;
	wxPanel* m_right;
};
