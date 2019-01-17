#pragma once
#include "cocos2d.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"

using namespace cocos2d;

namespace {
struct ActionInfo {
	float time = 0;
	bool down = false, up = false;
};
}

class Actor {
public:
	Actor() = default;
	Actor::~Actor() { delete sprite; }

	void load(std::string path, Vec2 position);
	void kill(float delay = 0.0f);

	virtual void update(float dt) {};

	void moveBy(Vec2 movement, float duration = 0);

	void bufferAction(std::string action);

	// Getters and Setters
	Sprite* getSprite() { return sprite; }
	PhysicsBody* getBody() { return sprite->getPhysicsBody(); }


protected:
	Sprite* sprite;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	virtual void updateActionBuffer();

	bool isActionPressed(std::string action);
	bool isActionDown(std::string action);
	bool isActionUp(std::string action);

	float actionPressedDuration(std::string action);
};

