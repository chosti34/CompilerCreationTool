#include "stdafx.h"
#include "StateFlag.h"
#include <cassert>

std::string ToString(StateFlag flag)
{
	switch (flag)
	{
	case StateFlag::Shift:
		return "Shift";
	case StateFlag::Push:
		return "Push";
	case StateFlag::Error:
		return "Error";
	case StateFlag::End:
		return "End";
	case StateFlag::Attribute:
		return "Attribute";
	default:
		assert(false);
		throw std::logic_error("can't convert unknown state flag to string");
	}
}

std::vector<StateFlag> GetAllStateFlags()
{
	return
	{
		StateFlag::End,
		StateFlag::Push,
		StateFlag::Error,
		StateFlag::Shift,
		StateFlag::Attribute
	};
}
