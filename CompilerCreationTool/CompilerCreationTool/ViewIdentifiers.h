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
		Info,
		Up,
		Down,
		Edit,
		Help,
		Clear,
		LogMessages,
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
