#pragma once
#include "../Parser/IParserLogger.h"
#include "../Codegen/ICodegenLogger.h"
#include <wx/textctrl.h>

class TextCtrlParserLogger : public IParserLogger
{
public:
	explicit TextCtrlParserLogger(wxTextCtrl* ctrl);

	void SetMask(int mask) override;
	int GetMask() const override;

	void Log(const std::string& message, MessageCategory category = All) override;
	void Clear() override;

private:
	wxTextCtrl* mTextCtrl;
	int mMask;
};

class TextCtrlCodegenLogger : public ICodegenLogger
{
public:
	explicit TextCtrlCodegenLogger(wxTextCtrl* ctrl);
	void Log(const std::string& message) override;

private:
	wxTextCtrl* mTextCtrl;
};
