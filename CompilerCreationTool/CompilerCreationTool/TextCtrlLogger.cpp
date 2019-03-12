#include "pch.h"
#include "TextCtrlLogger.h"
#include <cassert>

TextCtrlLogger::TextCtrlLogger(wxStyledTextCtrl* ctrl)
	: mTextCtrl(ctrl)
{
	assert(mTextCtrl);
}

void TextCtrlLogger::Log(const std::string& message)
{
	mTextCtrl->SetEditable(true);
	mTextCtrl->AppendText(message);
	mTextCtrl->SetEditable(false);
}

void TextCtrlLogger::Clear()
{
	mTextCtrl->SetEditable(true);
	mTextCtrl->ClearAll();
	mTextCtrl->SetEditable(false);
}
