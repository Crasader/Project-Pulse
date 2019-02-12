#pragma once
#include "cocos2d.h"

#include "Entity.h"

#include "Level.h"

namespace
{
struct ActionInfo
{
	float time = 0;
	bool down = false;
	bool up = false;
	float value = 0;
};
}

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

class Actor : public Entity
{
public:
	Actor() = default;
	Actor(const std::string &path, const cocos2d::Vec2 &position);
	Actor::~Actor() { delete sprite; }

	void bufferAction(const std::string &action);

	virtual bool doTerrainCollision(Retry::Level* level , const float &delta);

	int getHealth() { return (int) health; }
	int getMaxHealth() { return (int) maxHealth; }
	float getHealthRatio() { return health / maxHealth; }
	void decreaseHealth(const float &delta);
	void increaseHealth(const float &delta);

protected:
	float health = 100;
	float maxHealth = 100;

	int doJump = 0;
	bool onGround = false;

	std::unordered_map<std::string, cocos2d::Animation*> animations;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	virtual void updateActionBuffer();

	bool isActionPressed(const std::string &action);
	bool isActionDown(const std::string &action);
	bool isActionUp(const std::string &action);

	float actionPressedDuration(const std::string &action);
	float actionPressedValue(const std::string &action);

	float doSolidCollisionX(Retry::Level* level, const Rect &boundingBox);
	float doSolidCollisionY(Retry::Level* level, const Rect &boundingBox);

	float doPlatformCollisionX(Retry::Level* level, const Rect &boundingBox);
	float doPlatformCollisionY(Retry::Level* level, const Rect &boundingBox);
};

}