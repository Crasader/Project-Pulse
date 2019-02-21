#include "Actor.h"

#include "CameraManager.h"
#include "ControllerManager.h"
#include "KeyboardManager.h"

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
	if (velocity.y < -800) onGround = false;

	Rect boundingBox(sprite->convertToWorldSpace(hurtBox.getBoundingBox().origin), hurtBox.getBoundingBox().size);
	for (cocos2d::Node* n = sprite; n != nullptr; n = n->getParent())
		boundingBox.size = boundingBox.size * n->getScale();

	doSolidCollisionX(level, boundingBox);
	doSolidCollisionY(level, boundingBox);

	doPlatformCollisionX(level, boundingBox);
	doPlatformCollisionY(level, boundingBox);

	// Update the sprite's position along with the character's position
	setPosition(position);

	return position != lastPosition;
}

// TENTATIVE FUNCTIONS DO NOT USE UNLESS YOU KNOW WHAT THEY DO
void Actor::decreaseHealth(const float &delta)
{
	health -= 40 * delta;
	if (health < 0) health = 0;
}

void Actor::increaseHealth(const float &delta)
{
	health += 40 * delta;
	if (health > maxHealth) health = maxHealth;
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

float Actor::doSolidCollisionX(Retry::Level* level, const Rect &boundingBox)
{
	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));
	float incY = (boundingBox.getMaxY() - boundingBox.getMinY()) / ceil(getHeight() / (level->getTileSize() - 1));

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX)
	{
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m; j += incY)
		{
			Vec2 currentTile = Vec2(i, j - deltaPosition.y) / level->getTileSize();
			if (boundingBox.getMinX() < 0 || level->getCollisionDataAt(currentTile) & 0x01)
			{
				position.x = lastPosition.x;
				velocity.x = 0;

				return position.x;
			}
		}
	}
	return position.x;
}

float Actor::doSolidCollisionY(Retry::Level* level, const Rect &boundingBox)
{
	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));
	float incY = (boundingBox.getMaxY() - boundingBox.getMinY()) / ceil(getHeight() / (level->getTileSize() - 1));

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX)
	{
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m; j += incY)
		{
			Vec2 currentTile = Vec2(i - (position.x == lastPosition.x ? deltaPosition.x : 0), j) / level->getTileSize();
			if (boundingBox.getMinY() < 0 || level->getCollisionDataAt(currentTile) & 0x01)
			{
				if (!onGround && velocity.y < 0)
				{
					Camera::addTrauma(0.3f);
					Controller::vibrate(0.2f, 0.1f);
					onGround = true;
				}

				position.y = lastPosition.y;
				velocity.y = 0;
				return position.y;
			}
		}
	}
	return position.y;
}

float Actor::doPlatformCollisionX(Retry::Level* level, const Rect &boundingBox)
{

	
	return position.x;
}

float Actor::doPlatformCollisionY(Retry::Level* level, const Rect &boundingBox)
{
	if (velocity.y > 0 || 
		Controller::getAxis(ControllerButton::LEFT_STICK_DOWN) < -0.8f ||
		Keyboard::isKeyDown(KeyCode::S)) return position.y;

	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX)
	{
		Vec2 currentTile = Vec2(i - (position.x == lastPosition.x ? deltaPosition.x : 0), boundingBox.getMinY()) / level->getTileSize();
		if ((level->getCollisionDataAt(currentTile) & 0x02) &&
			!level->getCollisionDataAt(Vec2(currentTile.x, (boundingBox.getMinY() - deltaPosition.y) / level->getTileSize())))
		{
			if (!onGround)
			{
				Camera::addTrauma(0.3f);
				Controller::vibrate(0.2f, 0.1f);
				onGround = true;
			}

			position.y = lastPosition.y;
			velocity.y = 0;
			return position.y;
		}
	}
	return position.y;
}

}