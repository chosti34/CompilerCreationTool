#include "stdafx.h"
#include "Action.h"

Action::Action(
	const std::string& name, ActionType type,
	std::string message /* = "" */
)
	: mType(type)
	, mName(name)
	, mMessage(std::move(message))
{
}

void Action::SetName(const std::string& name)
{
	mName = name;
}

const std::string& Action::GetName() const
{
	return mName;
}

void Action::SetType(ActionType type)
{
	mType = type;
}

ActionType Action::GetType() const
{
	return mType;
}

void Action::SetMessage(std::string message)
{
	mMessage = std::move(message);
}

const std::string& Action::GetMessage() const
{
	return mMessage;
}
