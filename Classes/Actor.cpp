#include "Actor.h"

namespace Retry
{

Actor::Actor(const std::string &name, const cocos2d::Vec2 &position)
{
	init(name, position);
}

void Actor::bufferAction(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		actionBuffer[action];
}

void Actor::updateActionBuffer()
{

}

bool Actor::isActionPressed(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].pressed;
	return false;
}

bool Actor::isActionDown(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].down;
	return false;
}

bool Actor::isActionUp(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].up;
	return false;
}

float Actor::actionPressedDuration(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].time;
	return false;
}

}