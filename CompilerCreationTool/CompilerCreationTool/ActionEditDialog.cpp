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
	mMessageTextCtrl = new wxTextCtrl(messageStaticBoxSizer->GetStaticBox(), wxID_ANY);
	messageStaticBoxSizer->Add(mMessageTextCtrl, 1, wxEXPAND | wxALL, 3);
	leftSizer->Add(messageStaticBoxSizer, 0, wxEXPAND);

	wxStaticBoxSizer* actionsStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Actions");
	mActionsListbox = new wxListBox(actionsStaticBoxSizer->GetStaticBox(), wxID_ANY);
	actionsStaticBoxSizer->Add(mActionsListbox, 1, wxEXPAND | wxALL, 3);
	leftSizer->Add(actionsStaticBoxSizer, 1, wxEXPAND);

	wxStaticBoxSizer* descriptionStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Description");
	mDescriptionText = new wxStaticText(descriptionStaticBoxSizer->GetStaticBox(), wxID_ANY, "");
	descriptionStaticBoxSizer->Add(mDescriptionText, 1, wxEXPAND | wxALL, 3);
	rightSizer->Add(descriptionStaticBoxSizer, 1, wxEXPAND);

	// Создаем кнопки
	wxButton* okButton = new wxButton(panel, wxID_OK, wxT("Ok"));
	wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));

	// Связываем кнопки с sizer'ом
	buttonSizer->Add(okButton, 0);
	buttonSizer->Add(cancelButton, 0, wxLEFT, 5);

	panel->SetSizerAndFit(mainSizer);
	CentreOnParent();

	// Заполняем данными форму
	for (const ActionType& actionType : GetActionTypesList())
	{
		mActionsListbox->Append(ToPrettyString(actionType));
	}

	mActionsListbox->SetSelection(GetActionTypeIndex(action.GetType()));
	mMessageTextCtrl->SetValue(action.GetMessage());

	mDescriptionWrapWidth = rightSizer->GetSize().GetWidth();
	mDescriptionText->SetLabel(GetDescription(action.GetType()));
	mDescriptionText->Wrap(mDescriptionWrapWidth);

	// Связываем события с их обработчиками
	mActionsListbox->Bind(wxEVT_LISTBOX, &ActionEditDialog::OnListboxItemSelection, this);
}

ActionType ActionEditDialog::GetActionTypeSelection() const
{
	const int selection = mActionsListbox->GetSelection();
	assert(selection != wxNOT_FOUND);

	const auto& types = GetActionTypesList();
	assert(size_t(selection) < types.size());

	return types[selection];
}

wxString ActionEditDialog::GetActionMessage() const
{
	return mMessageTextCtrl->GetValue();
}

void ActionEditDialog::OnListboxItemSelection(wxCommandEvent& event)
{
	mDescriptionText->SetLabel(GetDescription(GetActionTypesList()[event.GetSelection()]));
	mDescriptionText->Wrap(mDescriptionWrapWidth);
	event.Skip(true);
}
