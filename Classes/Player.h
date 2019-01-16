#pragma once
#include "Actor.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"
#include "AudioManager.h"

class Player : public Actor {
public:
	Player(std::string path, Vec2 pos);

	void update(float delta);

	static Retry::KeyboardManager* keyIn;
	static Retry::MouseManager* mouseIn;
	static Retry::ControllerManager* controllerIn;
	static Retry::AudioManager* audio;
};

