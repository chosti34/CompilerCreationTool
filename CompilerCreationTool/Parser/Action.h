#pragma once
#include "IAction.h"

class Action : public IAction
{
public:
	explicit Action(const std::string& name, ActionType type);

	void SetName(const std::string& name) override;
	const std::string& GetName() const override;

	void SetType(ActionType type) override;
	ActionType GetType() const override;

private:
	std::string m_name;
	ActionType m_type;
};
