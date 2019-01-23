#pragma once
#include "Actor.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"
#include "AudioManager.h"

namespace {

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
	static std::unordered_map<std::string, KeyMap> actionMapping;

	void updateActionBuffer();
};

