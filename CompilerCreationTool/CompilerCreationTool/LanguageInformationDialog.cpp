#include "pch.h"
#include "LanguageInformationDialog.h"
#include "../Utils/string_utils.h"
#include <wx/statbox.h>

namespace
{
const wxColour gcOkForegroundColor = wxColour(19, 132, 15);
const wxColour gcWarningForegroundColor = wxColour(255, 0, 0);
}

LanguageInformationDialog::LanguageInformationDialog(wxWindow* parent, const LanguageInformation& info)
	: wxDialog(parent, wxID_ANY, wxT("Information About Language"))
	, m_info(info)
{
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* box = new wxStaticBox(panel, wxID_ANY, wxT("Language Info"));
	wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* languageBuildTimeLabel = new wxStaticText(panel, wxID_ANY, "Language Build Time:");
	wxStaticText* terminalsCountLabel = new wxStaticText(panel, wxID_ANY, "Terminals Count:");
	wxStaticText* actionsCountLabel = new wxStaticText(panel, wxID_ANY, "Actions Count:");
	wxStaticText* productionsCountLabel = new wxStaticText(panel, wxID_ANY, "Productions Count:");
	wxStaticText* emptyProductionsCountLabel = new wxStaticText(panel, wxID_ANY, "Empty Productions Count:");
	wxStaticText* leftRecursiveNonterminalsLabel = new wxStaticText(panel, wxID_ANY, "Left Recursive Nonterminals:");
	wxStaticText* acceptablesCrossingLabel = new wxStaticText(panel, wxID_ANY, "Acceptables Crossing Nonterminals:");

	leftSizer->Add(languageBuildTimeLabel, 0, wxALIGN_RIGHT | wxTOP | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(terminalsCountLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(actionsCountLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(productionsCountLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(emptyProductionsCountLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(leftRecursiveNonterminalsLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);
	leftSizer->Add(acceptablesCrossingLabel, 0, wxALIGN_RIGHT | wxLEFT | wxBOTTOM, 5);

	wxStaticText* languageBuildTimeText = new wxStaticText(panel, wxID_ANY, string_utils::TrimTrailingZerosAndPeriod(m_info.GetBuildTime().count()) + "s");
	wxStaticText* terminalsCountText = new wxStaticText(panel, wxID_ANY, std::to_string(info.GetTerminalsCount()));
	wxStaticText* actionsCountText = new wxStaticText(panel, wxID_ANY, std::to_string(info.GetActionsCount()));
	wxStaticText* productionsCountText = new wxStaticText(panel, wxID_ANY, std::to_string(info.GetProductionsCount()));
	wxStaticText* emptyProductionsCountText = new wxStaticText(panel, wxID_ANY, std::to_string(info.GetEmptyProductionsCount()));
	wxStaticText* leftRecursiveNonterminalsText = new wxStaticText(panel, wxID_ANY, info.GetLeftRecursiveNonterminals().empty() ?
		"none" : string_utils::JoinStrings(info.GetLeftRecursiveNonterminals()));
	wxStaticText* acceptablesCrossingText = new wxStaticText(panel, wxID_ANY, info.GetAcceptablesCrossingNonterminals().empty() ?
		"none" : string_utils::JoinStrings(info.GetAcceptablesCrossingNonterminals()));

	leftRecursiveNonterminalsText->SetForegroundColour(
		info.GetLeftRecursiveNonterminals().empty() ?
		gcOkForegroundColor : gcWarningForegroundColor
	);
	acceptablesCrossingText->SetForegroundColour(
		info.GetAcceptablesCrossingNonterminals().empty() ?
		gcOkForegroundColor : gcWarningForegroundColor
	);

	rightSizer->Add(languageBuildTimeText, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(terminalsCountText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(actionsCountText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(productionsCountText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(emptyProductionsCountText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(leftRecursiveNonterminalsText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);
	rightSizer->Add(acceptablesCrossingText, 0, wxALIGN_LEFT | wxBOTTOM | wxRIGHT, 5);

	staticBoxSizer->Add(leftSizer, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);
	staticBoxSizer->Add(rightSizer, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);

	sizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);
	panel->SetSizer(sizer);

	CentreOnParent();
	SetDoubleBuffered(true);
}
