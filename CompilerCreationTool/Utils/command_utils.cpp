#include "stdafx.h"
#include "command_utils.h"
#include <Windows.h>
#include <shellapi.h>

bool command_utils::RunCommand(const std::wstring& name, const std::wstring& params)
{
	SHELLEXECUTEINFO sei;

	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.lpVerb = L"open";
	sei.lpFile = name.c_str();
	sei.lpParameters = params.c_str();
	sei.lpDirectory = NULL;
	sei.nShow = SW_HIDE;
	sei.hInstApp = NULL;
	bool ret = ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE);

	return ret;
}
