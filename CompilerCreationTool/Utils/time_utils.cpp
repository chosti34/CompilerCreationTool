#include "stdafx.h"
#include "time_utils.h"
#include <iomanip>
#include <sstream>
#include <ctime>

std::string time_utils::GetCurrentTimeAsString()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream os;
	os << std::put_time(&tm, "%H:%M:%S");
	return os.str();
}
