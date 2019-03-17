#pragma once
#include "../Parser/IParserLogger.h"
#include <wx/textctrl.h>

class TextCtrlLogger : public IParserLogger
{
public:
	TextCtrlLogger(wxTextCtrl* ctrl);

	void Log(const std::string& message) override;
	void Clear() override;

private:
	wxTextCtrl* mTextCtrl;
};
