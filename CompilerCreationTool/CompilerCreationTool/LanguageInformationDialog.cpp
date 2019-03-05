#include "pch.h"
#include "LanguageInformationDialog.h"

LanguageInformationDialog::LanguageInformationDialog(wxWindow* parent, const LanguageInformation& info)
	: wxDialog(parent, wxID_ANY, wxT("Language Information"))
	, m_info(info)
{
}
