#include "pch.h"
#include "TextCtrlLogger.h"
#include <cassert>

TextCtrlLogger::TextCtrlLogger(wxTextCtrl* ctrl)
	: mTextCtrl(ctrl)
{
	assert(mTextCtrl);
}

void TextCtrlLogger::Log(const std::string& message)
{
	mTextCtrl->Freeze();
	mTextCtrl->SetEditable(true);
	mTextCtrl->AppendText(message);
	mTextCtrl->SetEditable(false);
	mTextCtrl->Thaw();
	mTextCtrl->ShowPosition(mTextCtrl->GetLastPosition());
}

void TextCtrlLogger::Clear()
{
	mTextCtrl->Freeze();
	mTextCtrl->SetEditable(true);
	mTextCtrl->Clear();
	mTextCtrl->SetEditable(false);
	mTextCtrl->Thaw();
	mTextCtrl->ShowPosition(mTextCtrl->GetLastPosition());
}
