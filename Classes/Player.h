#pragma once
#include "Actor.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"
#include "AudioPlayer.h"

namespace {

struct KeyMap {
	std::vector<Retry::KeyCode> kButtons;
	std::vector<Retry::MouseButton> mButtons;
	std::vector<Retry::ControllerButton> cButtons;
};
}

namespace Retry
{

class Player : public Actor
{
public:
	Player(std::string path, cocos2d::Vec2 pos);

	void update(float delta);

private:
	static std::unordered_map<std::string, KeyMap> actionMapping;

	void updateActionBuffer();
};

}

