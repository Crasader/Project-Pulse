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
	float value = 0;
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

	void bufferAction(const std::string &action);

protected:
	std::unordered_map<std::string, ActionInfo> actionBuffer;

	std::unordered_map<std::string, cocos2d::Animation*> animations;

	virtual void updateActionBuffer();

	bool isActionPressed(const std::string &action);
	bool isActionDown(const std::string &action);
	bool isActionUp(const std::string &action);

	float actionPressedDuration(const std::string &action);
	float actionPressedValue(const std::string &action);
};

}