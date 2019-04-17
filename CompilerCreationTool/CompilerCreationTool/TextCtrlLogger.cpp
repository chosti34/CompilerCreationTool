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

TextCtrlLogger::TextCtrlLogger(wxTextCtrl* ctrl)
	: mTextCtrl(ctrl)
	, mMask(All)
{
	assert(mTextCtrl);
}

void TextCtrlLogger::SetMask(int mask)
{
	mMask = mask;
}

int TextCtrlLogger::GetMask() const
{
	return mMask;
}

void TextCtrlLogger::Log(const std::string& message, MessageCategory category /* = All */)
{
	if (mMask & category)
	{
		WorkWithTextCtrl(*mTextCtrl, [&message](wxTextCtrl& ctrl) {
			ctrl.AppendText(message);
		});
	}
}

void TextCtrlLogger::Clear()
{
	WorkWithTextCtrl(*mTextCtrl, [](wxTextCtrl& ctrl) {
		ctrl.Clear();
	});
}
