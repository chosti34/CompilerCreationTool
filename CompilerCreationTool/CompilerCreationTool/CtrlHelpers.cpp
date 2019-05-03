#include "pch.h"
#include "CtrlHelpers.h"
#include <wx/stc/stc.h>

void SetupStyledTextCtrlMargins(wxStyledTextCtrl& ctrl)
{
	const int cLeftMarginIndex = 0;
	const int cRightMarginIndex = 1;

	ctrl.SetMarginWidth(cLeftMarginIndex, 15);
	ctrl.SetMarginType(cLeftMarginIndex, wxSTC_MARGIN_COLOUR);
	ctrl.SetMarginBackground(cLeftMarginIndex, wxColour(245, 245, 245));

	ctrl.SetMarginWidth(cRightMarginIndex, 20);
	ctrl.StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 191, 255));
	ctrl.StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(255, 255, 255));
	ctrl.SetMarginType(cRightMarginIndex, wxSTC_MARGIN_NUMBER);

	ctrl.SetMarginLeft(10);
}
