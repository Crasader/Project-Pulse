#ifndef CONTROLLER_MANAGER
#define CONTROLLER_MANAGER

#include <ctime>
#include "cocos2d.h"

namespace Retry {

namespace {
struct ControllerInfo;
};

enum class ControllerButton;

class ControllerManager {
private:
	static ControllerManager* instance;
	ControllerManager() = default;
public:
	static ControllerManager* getInstance();

	void refresh();

	void createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node);

	void updateButton(ControllerButton key, bool isPressed, int id);

	bool isButtonPressed(ControllerButton key, int id = 0);
	bool isButtonDown(ControllerButton key, int id = 0);
	bool isButtonUp(ControllerButton key, int id = 0);

	float buttonPressedDuration(ControllerButton key, int id = 0);

	cocos2d::Vec2 getLStick(int id = 0);
	float getLStickX(int id = 0);
	float getLStickY(int id = 0);

	cocos2d::Vec2 getDeltaLStick(int id = 0);
	float getDeltaLStickX(int id = 0);
	float getDeltaLStickY(int id = 0);

	cocos2d::Vec2  getRStick(int id = 0);
	float getRStickX(int id = 0);
	float getRStickY(int id = 0);

	cocos2d::Vec2  getDeltaRStick(int id = 0); 
	float getDeltaRStickX(int id = 0);
	float getDeltaRStickY(int id = 0);

	float getLTrigger(int id = 0); 
	float getDeltaLTrigger(int id = 0);

	float getRTrigger(int id = 0); 
	float getDeltaRTrigger(int id = 0); 

private:
	//static std::unordered_map<Retry::ControllerButton, clock_t> buttons;
	//static std::unordered_map<Retry::ControllerButton, bool> buttonsLast;

	//static cocos2d::Vec2 leftStick, rightStick;
	//static cocos2d::Vec2 deltaLeftStick, deltaRightStick;

	//static float leftTrigger, rightTrigger;
	//static float deltaLeftTrigger, deltaRightTrigger;

	static std::map<int, ControllerInfo> controllers;

	static clock_t currentTime;
};

enum class ControllerButton {
	A,
	B,
	X,
	Y,
	LEFT_BUMBER,
	RIGHT_BUMPER,
	SELECT,
	START,
	GUIDE,
	LEFT_STICK,
	RIGHT_STICK,
	DPAD_LEFT,
	DPAD_RIGHT,
	DPAD_UP,
	DPAD_DOWN
};

namespace {
struct ControllerInfo {

	ControllerInfo() = default;
	ControllerInfo(int id, std::string name) {
		this->id = id;
		this->name = name;
	}

	int id = -1;
	std::string name = std::string();

	std::unordered_map<Retry::ControllerButton, clock_t> buttons;
	std::unordered_map<Retry::ControllerButton, bool> buttonsLast;

	cocos2d::Vec2 leftStick = cocos2d::Vec2(), rightStick = cocos2d::Vec2();
	cocos2d::Vec2 deltaLeftStick = cocos2d::Vec2(), deltaRightStick = cocos2d::Vec2();

	float leftTrigger = 0, rightTrigger = 0;
	float deltaLeftTrigger = 0, deltaRightTrigger = 0;
};
}

};

#endif