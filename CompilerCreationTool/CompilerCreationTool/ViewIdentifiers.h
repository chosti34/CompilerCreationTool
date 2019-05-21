#pragma once

namespace Buttons
{
	// ”никальные идентификаторы дл€ всех кнопок в приложении
	enum ID
	{
		// ћеню
		New = 0,
		Open,
		Save,
		SaveAs,
		Build,
		Run,
		Cancel,
		Info,
		Up,
		Down,
		Edit,
		Help,
		Clear,
		LogMessages,
		EnableCodegen,
		Count
	};
}

namespace StatusBarFields
{
	// ”никальные идентификаторы дл€ полей Status-бара
	enum ID
	{
		HelpInfo = 0,
		Line,
		Column,
		Ch,
		ContextInfo,
		Count
	};
}

namespace Menubar
{
	enum ID
	{
		File = 0,
		Edit,
		View,
		Parser,
		Help,
		Count
	};
}
