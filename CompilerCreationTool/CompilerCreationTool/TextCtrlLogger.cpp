#include "pch.h"
#include "TextCtrlLogger.h"
#include <cassert>

namespace
{
void WorkWithTextCtrl(wxTextCtrl& ctrl, std::function<void(wxTextCtrl&)> && fn)
{
	ctrl.Freeze();
	ctrl.SetEditable(true);
	fn(ctrl);
	ctrl.SetEditable(false);
	ctrl.Thaw();
	ctrl.ShowPosition(ctrl.GetLastPosition());
}
}

TextCtrlParserLogger::TextCtrlParserLogger(wxTextCtrl* ctrl)
	: mTextCtrl(ctrl)
	, mMask(All)
{
	assert(mTextCtrl);
}

void TextCtrlParserLogger::SetMask(int mask)
{
	mMask = mask;
}

int TextCtrlParserLogger::GetMask() const
{
	return mMask;
}

void TextCtrlParserLogger::Log(const std::string& message, MessageCategory category /* = All */)
{
	if (mMask & category)
	{
		WorkWithTextCtrl(*mTextCtrl, [&message](wxTextCtrl& ctrl) {
			ctrl.AppendText(message);
		});
	}
}

void TextCtrlParserLogger::Clear()
{
	WorkWithTextCtrl(*mTextCtrl, [](wxTextCtrl& ctrl) {
		ctrl.Clear();
	});
}

TextCtrlCodegenLogger::TextCtrlCodegenLogger(wxTextCtrl* ctrl)
	: mTextCtrl(ctrl)
{
	assert(ctrl);
}

void TextCtrlCodegenLogger::Log(const std::string& message)
{
	WorkWithTextCtrl(*mTextCtrl, [&message](wxTextCtrl& ctrl) {
		ctrl.AppendText(message);
	});
}
