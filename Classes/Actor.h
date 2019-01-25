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

	void moveBy(cocos2d::Vec2 movement);

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

	cocos2d::Vec2 getPosition()
	{
		return position;
	}
	cocos2d::Vec2 getVelocity()
	{
		return velocity;
	}
	cocos2d::Vec2 getAcceleration()
	{
		return acceleration;
	}

	void setPosition(cocos2d::Vec2 position)
	{
		this->position = position;
		sprite->setPosition(position);
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

	void initAnimation(std::string action, std::string file, cocos2d::Vec2 startCell, cocos2d::Vec2 frameSize, int numFrames);
	void runAnimation(std::string action, float totalTime = -1);

protected:
	cocos2d::Sprite* sprite;
	cocos2d::Vec2 position;
	cocos2d::Vec2 velocity;
	cocos2d::Vec2 acceleration;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	//std::unordered_map<std::string, std::vector<cocos2d::PolygonInfo>> animations;
	std::unordered_map<std::string, cocos2d::Animation*> animations;
	//cocos2d::Animate* currentAnimation = nullptr;

	float animTime, totalTime;
	std::string currentAction;

	virtual void updateActionBuffer();

	bool isActionPressed(std::string action);
	bool isActionDown(std::string action);
	bool isActionUp(std::string action);

	float actionPressedDuration(std::string action);
};

}