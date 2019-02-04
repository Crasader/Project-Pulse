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

	void addButtonToMapping(const std::string &action, const Retry::KeyCode &button);
	void addButtonToMapping(const std::string &action, const Retry::MouseButton &button);
	void addButtonToMapping(const std::string &action, const Retry::ControllerButton &button);

	void removeButtonFromMapping(const std::string &action, const Retry::KeyCode &button);
	void removeButtonFromMapping(const std::string &action, const Retry::MouseButton &button);
	void removeButtonFromMapping(const std::string &action, const Retry::ControllerButton &button);
	

private:
	std::unordered_map<std::string, KeyMap> actionMapping;

	void updateActionBuffer();
};

}

