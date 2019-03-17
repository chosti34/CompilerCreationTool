#pragma once
#include <string>

namespace command_utils
{
// Will wait until command execution is done
bool RunCommand(const std::wstring& name, const std::wstring& params);
}
