#include "pch.h"
#include "ActionEditDialog.h"
#include <wx/statbox.h>

namespace
{
}

ActionEditDialog::ActionEditDialog(wxWindow* parent, const IAction& action)
	: wxDialog(parent, wxID_ANY, wxT("Edit action '" + action.GetName() + "'"),
		wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

	panelSizer->Add(leftSizer, 1, wxEXPAND);
	panelSizer->Add(rightSizer, 1, wxEXPAND);

	wxStaticBox* box = new wxStaticBox(panel, wxID_ANY, wxT("Choose function"));
	wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	mComboBox = new wxComboBox(box, wxID_ANY);
	for (const ActionType& type : GetActionTypesList())
	{
		mComboBox->AppendString(ToString(type));
	}

	mComboBox->SetSelection(GetActionTypeIndex(action.GetType()));
	mComboBox->SetEditable(false);
	staticBoxSizer->Add(mComboBox, 1, wxEXPAND | wxALL, 5);
	leftSizer->Add(staticBoxSizer, 0, wxEXPAND);

	wxStaticBox* actionsStaticBox = new wxStaticBox(panel, wxID_ANY, "Actions");
	wxStaticBoxSizer* actionsStaticBoxSizer = new wxStaticBoxSizer(actionsStaticBox, wxHORIZONTAL);
	wxListBox* listbox = new wxListBox(actionsStaticBox, wxID_ANY);
	actionsStaticBoxSizer->Add(listbox, 1, wxEXPAND | wxALL, 5);
	leftSizer->Add(actionsStaticBoxSizer, 1, wxEXPAND);

	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(okButton);
	buttonSizer->Add(cancelButton, 0, wxLEFT, 5);

	mainSizer->Add(panelSizer, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(buttonSizer, 1, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 5);
	panel->SetSizer(mainSizer);

	CentreOnParent();
}

ActionType ActionEditDialog::GetActionTypeSelection() const
{
	const int selection = mComboBox->GetSelection();
	const auto& types = GetActionTypesList();
	assert(size_t(selection) < types.size());
	return types[selection];
}
