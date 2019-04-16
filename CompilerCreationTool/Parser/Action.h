#pragma once
#include "IAction.h"

class Action : public IAction
{
public:
	explicit Action(const std::string& name, ActionType type, std::string message = "");

	void SetName(const std::string& name) override;
	const std::string& GetName() const override;

	void SetType(ActionType type) override;
	ActionType GetType() const override;

	void SetMessage(std::string message) override;
	const std::string& GetMessage() const override;

private:
	std::string mName;
	std::string mMessage;
	ActionType mType;
};
