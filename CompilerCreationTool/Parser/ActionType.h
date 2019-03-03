#pragma once
#include <vector>
#include <string>

enum class ActionType
{
	None,
	PrintHello,
	PrintWorld
};

const std::vector<ActionType>& GetAllActionTypes();
int GetActionTypeIndex(ActionType type);
std::string ToString(ActionType type);
