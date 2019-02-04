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

void Actor::doTerrainCollision(Retry::Collision::Body* terrain, const float &delta)
{
	auto block = Retry::Collision::worldSpaceRect(terrain->getHitBox(), terrain->getRects().front());

	if (velocity.y < -800) hasLanded = onGround = false;
	
	float dist = (position - block.position).getLengthSq();
	float length = (Vec2(getWidth(), getHeight()) + block.size).getLengthSq();
	if (dist > length) return;

	auto temp = position;
	setPosition(Vec2(lastPosition.x, temp.y));
	if (this->getHurtBox()->isCollidingWith(terrain))
	{
		if (position.y < block.position.y + block.size.height * 2 &&
			position.y + this->getHeight() > block.position.y)
		{
			if (velocity.y < 0)
			{
				if (!onGround && !hasLanded) hasLanded = true;
				onGround = true;
				doJump = 0;
			}
			temp.y = lastPosition.y;
			velocity.y = 0;
		}
	}
	setPosition(Vec2(temp.x, lastPosition.y));
	if (this->getHurtBox()->isCollidingWith(terrain))
	{
		if (position.x < block.position.x + block.size.width * 2 &&
			position.x + this->getWidth() > block.position.x)
		{
			temp.x = lastPosition.x;
			velocity.x = 0;
		}

	}
	position = temp;
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