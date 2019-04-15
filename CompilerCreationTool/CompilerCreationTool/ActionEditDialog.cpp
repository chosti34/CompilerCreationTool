#include "pch.h"
#include "ActionEditDialog.h"
#include <wx/statbox.h>


ActionEditDialog::ActionEditDialog(wxWindow* parent, const IAction& action)
	: wxDialog(parent, wxID_ANY, wxT("Edit action '" + action.GetName() + "'"),
		wxDefaultPosition, wxSize(410, 330), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	wxPanel* panel = new wxPanel(this);

	// Главный sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// Два горизонтальных sizer'а: один для панелей (сверху), другой для кнопок (снизу)
	wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	// Добавляем sizer'ы
	mainSizer->Add(panelSizer, 1, wxEXPAND | wxALL, 3);
	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 6);

	// Левый и правый sizer'ы для панелей
	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(leftSizer, 1, wxEXPAND | wxALL, 3);
	panelSizer->Add(rightSizer, 1, wxEXPAND | wxALL, 3);

	wxStaticBoxSizer* messageStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Message");
	wxTextCtrl* messageCtrl = new wxTextCtrl(messageStaticBoxSizer->GetStaticBox(), wxID_ANY);
	messageStaticBoxSizer->Add(messageCtrl, 1, wxEXPAND | wxALL, 3);
	leftSizer->Add(messageStaticBoxSizer, 0, wxEXPAND);

	wxStaticBoxSizer* actionsStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Actions");
	wxListBox* listbox = new wxListBox(actionsStaticBoxSizer->GetStaticBox(), wxID_ANY);
	actionsStaticBoxSizer->Add(listbox, 1, wxEXPAND | wxALL, 3);
	leftSizer->Add(actionsStaticBoxSizer, 1, wxEXPAND);

	wxStaticBoxSizer* descriptionStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Description");
	wxStaticText* description = new wxStaticText(descriptionStaticBoxSizer->GetStaticBox(), wxID_ANY, "askdjaslkd");
	descriptionStaticBoxSizer->Add(description, 1, wxEXPAND | wxALL, 3);
	rightSizer->Add(descriptionStaticBoxSizer, 1, wxEXPAND);

	// Создаем кнопки
	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	// Связываем кнопки с sizer'ом
	buttonSizer->Add(okButton, 0);
	buttonSizer->Add(cancelButton, 0, wxLEFT, 5);

	panel->SetSizerAndFit(mainSizer);
	CentreOnParent();

	/*wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
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
	panel->SetSizer(mainSizer);*/
}

ActionType ActionEditDialog::GetActionTypeSelection() const
{
	const int selection = mComboBox->GetSelection();
	const auto& types = GetActionTypesList();
	assert(size_t(selection) < types.size());
	return types[selection];
}
