#pragma once
#include "../Parser/IAction.h"
#include <wx/dialog.h>

class ActionEditDialog : public wxDialog
{
public:
	ActionEditDialog(wxWindow* parent, const IAction& action);
	ActionType GetActionTypeSelection() const;

private:
	wxComboBox* m_combo;
};
