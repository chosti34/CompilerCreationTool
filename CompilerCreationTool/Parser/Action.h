#pragma once
#include "IAction.h"

class Action : public IAction
{
public:
	explicit Action(const std::string& name, ActionType type);

	const std::string& GetName() const override;
	ActionType GetType() const override;

private:
	std::string m_name;
	ActionType m_type;
};
