#pragma once
#include "../Parser/IParserLogger.h"
#include <wx/textctrl.h>

class TextCtrlLogger : public IParserLogger
{
public:
	explicit TextCtrlLogger(wxTextCtrl* ctrl);

	void SetMask(int mask) override;
	int GetMask() const override;

	void Log(const std::string& message, MessageCategory category = All) override;
	void Clear() override;

private:
	wxTextCtrl* mTextCtrl;
	int mMask;
};
