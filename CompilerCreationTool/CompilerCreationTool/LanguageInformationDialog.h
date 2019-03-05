#pragma once
#include "LanguageInformation.h"
#include <wx/dialog.h>

class LanguageInformationDialog : public wxDialog
{
public:
	LanguageInformationDialog(wxWindow* parent, const LanguageInformation& info);

private:
	const LanguageInformation& m_info;
};
