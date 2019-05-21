#pragma once

namespace Buttons
{
	// ���������� �������������� ��� ���� ������ � ����������
	enum ID
	{
		// ����
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
	// ���������� �������������� ��� ����� Status-����
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
