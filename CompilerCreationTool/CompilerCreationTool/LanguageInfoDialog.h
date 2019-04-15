#pragma once
#include "LanguageInfo.h"
#include <wx/dialog.h>

class LanguageInfoDialog : public wxDialog
{
public:
	LanguageInfoDialog(wxWindow* parent, const LanguageInfo& info);

private:
	const LanguageInfo& m_info;
};
