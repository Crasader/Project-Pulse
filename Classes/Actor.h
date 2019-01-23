#pragma once
#include "cocos2d.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"

namespace {
struct ActionInfo {
	float time = 0;
	bool down = false;
	bool up = false;
	bool pressed = false;
};
}

namespace Retry
{

class Actor
{
public:
	Actor() = default;
	Actor::~Actor()
	{
		delete sprite;
	}

	void load(std::string path, cocos2d::Vec2 position);
	void kill(float delay = 0.0f);

	virtual void update(float dt)
	{};

	void moveBy(cocos2d::Vec2 movement, float duration = 0);

	void bufferAction(std::string action);

	// Getters and Setters
	cocos2d::Sprite* getSprite()
	{
		return sprite;
	}
	cocos2d::PhysicsBody* getBody()
	{
		return sprite->getPhysicsBody();
	}

	cocos2d::Vec2 getVelocity()
	{
		return velocity;
	}
	cocos2d::Vec2 getAcceleration()
	{
		return acceleration;
	}

	void setVelocity(cocos2d::Vec2 velocity)
	{
		this->velocity = velocity;
	}
	void setVelocity(float x, float y)
	{
		this->velocity = cocos2d::Vec2(x, y);
	}

	void setAcceleration(cocos2d::Vec2 acceleration)
	{
		this->acceleration = acceleration;
	}
	void setAcceleration(float x, float y)
	{
		this->acceleration = cocos2d::Vec2(x, y);
	}


protected:
	cocos2d::Sprite* sprite;
	cocos2d::Vec2 velocity;
	cocos2d::Vec2 acceleration;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	virtual void updateActionBuffer();

	bool isActionPressed(std::string action);
	bool isActionDown(std::string action);
	bool isActionUp(std::string action);

	float actionPressedDuration(std::string action);
};

}