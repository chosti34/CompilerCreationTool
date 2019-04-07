#pragma once
#include <string>
#include <vector>

enum class StateFlag
{
	End = 0,
	Push,
	Error,
	Shift,
	Attribute
};

std::string ToString(StateFlag flag);
std::vector<StateFlag> GetAllStateFlags();
