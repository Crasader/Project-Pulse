#pragma once
#include "cocos2d.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioManager.h"

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

	static Retry::KeyboardManager* keyIn;
	static Retry::MouseManager* mouseIn;
	static Retry::AudioManager* audio;
};

