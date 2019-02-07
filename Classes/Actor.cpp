#include "Actor.h"

#include "CameraManager.h"
#include "ControllerManager.h"

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

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
	Vec2 deltaPos = position - lastPosition;

	bool doCollide = false;

	Rect boundingBox(sprite->convertToWorldSpace(hurtBox.getBoundingBox().origin), hurtBox.getBoundingBox().size);
	for (cocos2d::Node* n = sprite; n != nullptr; n = n->getParent())
		boundingBox.size = boundingBox.size * n->getScale();


	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / 2.f;
	float incY = (boundingBox.getMaxY() - boundingBox.getMinY()) / 2.f;

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX)
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m; j += incY)
		{
			Vec2 currentTile = Vec2(i, j - deltaPos.y) / level->getTileSize();
			if (level->getCollisionDataAt(currentTile))
			{
				temp.x = lastPosition.x;
				velocity.x = 0;
				doCollide = true;
			}

			currentTile = Vec2(i - (temp.x == lastPosition.x ? deltaPos.x : 0), j) / level->getTileSize();
			if (level->getCollisionDataAt(currentTile))
			{
				if (!onGround && velocity.y < 0)
				{
					Camera::addTrauma(0.3f);
					Controller::vibrate(0.2f, 0.1f);
					onGround = true;
				}

				temp.y = lastPosition.y;
				velocity.y = 0;
				doCollide = true;
			}
			if (doCollide) break;
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