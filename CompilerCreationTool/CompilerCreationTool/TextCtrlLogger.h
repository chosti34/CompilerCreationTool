#pragma once
#include "../Parser/IParserLogger.h"
#include <wx/stc/stc.h>

class TextCtrlLogger : public IParserLogger
{
public:
	TextCtrlLogger(wxStyledTextCtrl* ctrl);

	void Log(const std::string& message) override;
	void Clear() override;

private:
	wxStyledTextCtrl* mTextCtrl;
};
