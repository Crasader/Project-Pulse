#pragma once
#include "cocos2d.h"

#include "Entity.h"

namespace
{
struct ActionInfo
{
	float time = 0;
	bool down = false;
	bool up = false;
	bool pressed = false;
};
}

namespace Retry
{

class Actor : public Entity
{
public:
	Actor() = default;
	Actor(const std::string &path, const cocos2d::Vec2 &position);
	Actor::~Actor() { delete sprite; }

	void bufferAction(std::string action);

protected:
	std::unordered_map<std::string, ActionInfo> actionBuffer;

	std::unordered_map<std::string, cocos2d::Animation*> animations;

	virtual void updateActionBuffer();

	bool isActionPressed(std::string action);
	bool isActionDown(std::string action);
	bool isActionUp(std::string action);

	float actionPressedDuration(std::string action);
};

}