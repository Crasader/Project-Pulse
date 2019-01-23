#pragma once
#include "Actor.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"
#include "AudioManager.h"

namespace {
using namespace Retry;
struct KeyMap {
	std::vector<KeyCode> kButtons;
	std::vector<MouseButton> mButtons;
	std::vector<ControllerButton> cButtons;
};
}

class Player : public Actor {
public:
	Player(std::string path, Vec2 pos);

	void update(float delta);

private:
	static Retry::KeyboardManager* keyIn;
	static Retry::MouseManager* mouseIn;
	static Retry::ControllerManager* controllerIn;
	static Retry::AudioManager* audio;

	static std::unordered_map<std::string, KeyMap> actionMapping;

	void updateActionBuffer();
};

