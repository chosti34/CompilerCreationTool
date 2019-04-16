#pragma once
#include "../Parser/IAction.h"
#include <wx/dialog.h>

class ActionEditDialog : public wxDialog
{
public:
	ActionEditDialog(wxWindow* parent, const IAction& action);

	ActionType GetActionTypeSelection() const;
	wxString GetActionMessage() const;

private:
	void OnListboxItemSelection(wxCommandEvent& event);

private:
	wxTextCtrl* mMessageTextCtrl;
	wxStaticText* mDescriptionText;
	wxListBox* mActionsListbox;
	int mDescriptionWrapWidth;
};
