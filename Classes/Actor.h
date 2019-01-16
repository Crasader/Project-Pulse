#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class Actor {
public:
	Actor() = default;
	Actor::~Actor() { delete sprite; }

	void load(std::string path, Vec2 position);
	void kill(float delay = 0.0f);

	virtual void update(float dt) {};

	void moveBy(Vec2 movement, float duration = 0);

	// Getters and Setters
	Sprite* getSprite() { return sprite; }
	PhysicsBody* getBody() { return sprite->getPhysicsBody(); }

protected:
	Sprite* sprite;
};

