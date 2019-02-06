#include "Actor.h"

#include "CameraManager.h"
#include "ControllerManager.h"

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

bool Actor::doTerrainCollision(Retry::Level* level, const float &delta)
{
	if (velocity.y < -800) hasLanded = false;

	Vec2 temp = position;
	bool doCollide = false;

	cocos2d::Director::getInstance()->getRunningScene()->getChildByName("test")->setPosition(position);

	Vec2 loLeft = Vec2(int(lastPosition.x) / level->getTileSize(), int(position.y) / level->getTileSize());
	if (level->getCollisionDataAt(loLeft))
	{
		temp.y = lastPosition.y;
		velocity.y = 0;
		doCollide = true;

		onGround = true;
	}

	loLeft = Vec2((int) position.x / level->getTileSize(), (int) lastPosition.y / level->getTileSize());
	if (level->getCollisionDataAt(loLeft))
	{
		temp.x = lastPosition.x;
		velocity.x = 0;
		doCollide = true;
	}

	setPosition(temp);

	return temp != position;
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