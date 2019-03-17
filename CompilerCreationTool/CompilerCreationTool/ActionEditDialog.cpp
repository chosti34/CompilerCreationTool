#include "pch.h"
#include "ActionEditDialog.h"
#include <wx/statbox.h>

namespace
{
const wxSize DIALOG_SIZE = { 300, 140 };
}

ActionEditDialog::ActionEditDialog(wxWindow* parent, const IAction& action)
	: wxDialog(parent, wxID_ANY, wxT("Configure Action"),
		wxDefaultPosition, DIALOG_SIZE, wxDEFAULT_DIALOG_STYLE)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* box = new wxStaticBox(panel, wxID_ANY, wxT("Choose function"));
	wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	m_combo = new wxComboBox(box, wxID_ANY);

	for (const ActionType& type : GetActionTypesList())
	{
		m_combo->AppendString(ToString(type));
	}

	m_combo->SetSelection(GetActionTypeIndex(action.GetType()));
	m_combo->SetEditable(false);

	staticBoxSizer->Add(m_combo, 1, wxEXPAND | wxALL, 5);
	vSizer->Add(staticBoxSizer, 0, wxEXPAND | wxALL, 5);

	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(okButton);
	buttonSizer->Add(cancelButton, 0, wxLEFT, 5);
	vSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 5);

	panel->SetSizer(vSizer);
	CentreOnParent();
}

ActionType ActionEditDialog::GetActionTypeSelection() const
{
	const int selection = m_combo->GetSelection();
	const auto& types = GetActionTypesList();
	assert(size_t(selection) < types.size());
	return types[selection];
}
