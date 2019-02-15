#pragma once

namespace StateFlags
{
	enum
	{
		End			= 1 << 0,
		Push		= 1 << 1,
		Error		= 1 << 2,
		Shift		= 1 << 3,
		Attribute	= 1 << 4,
	};
}
