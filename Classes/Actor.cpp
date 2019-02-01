#include "Actor.h"

namespace Retry
{

Actor::Actor(const std::string &name, const cocos2d::Vec2 &position)
{
	init(name, position);
}

void Actor::bufferAction(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		actionBuffer[action];
}

void Actor::updateActionBuffer()
{

}

bool Actor::isActionPressed(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].value != 0;
	return false;
}

bool Actor::isActionDown(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].down;
	return false;
}

bool Actor::isActionUp(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].up;
	return false;
}

float Actor::actionPressedDuration(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].time;
	return false;
}

float Actor::actionPressedValue(const std::string &action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].value;
	return 0;
}

}