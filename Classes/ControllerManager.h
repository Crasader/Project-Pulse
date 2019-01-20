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
	void updateAxis(ControllerButton axis, float x, float dx, int id);

	bool isButtonPressed(ControllerButton key, int id = 0);
	bool isButtonDown(ControllerButton key, int id = 0);
	bool isButtonUp(ControllerButton key, int id = 0);

	float buttonPressedDuration(ControllerButton key, int id = 0);

	bool isAxisPressed(ControllerButton axis, int id = 0);
	bool isAxisDown(ControllerButton axis, int id = 0);
	bool isAxisUp(ControllerButton axis, int id = 0);

	cocos2d::Vec2 getLStick(int id = 0);
	float getLStickX(int id = 0);
	float getLStickY(int id = 0);

	cocos2d::Vec2 getDeltaLStick(int id = 0);
	float getDeltaLStickX(int id = 0);
	float getDeltaLStickY(int id = 0);

	cocos2d::Vec2 getRStick(int id = 0);
	float getRStickX(int id = 0);
	float getRStickY(int id = 0);

	cocos2d::Vec2 getDeltaRStick(int id = 0); 
	float getDeltaRStickX(int id = 0);
	float getDeltaRStickY(int id = 0);

	float getLTrigger(int id = 0); 
	float getDeltaLTrigger(int id = 0);

	float getRTrigger(int id = 0); 
	float getDeltaRTrigger(int id = 0); 

	void setDeadZone(ControllerButton key, float t, int id = 0);

	bool doUseController() { return useController; }
	void setUseController(bool useController) { this->useController = useController; }

private:
	static std::map<int, ControllerInfo> controllers;

	static clock_t currentTime;

	static bool useController;
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
	LEFT_STICK,
	RIGHT_STICK,
	DPAD_UP,
	DPAD_RIGHT,
	DPAD_DOWN,
	DPAD_LEFT,
	LEFT_STICK_X, AXIS_START = LEFT_STICK_X,
	LEFT_STICK_Y,
	RIGHT_STICK_X,
	RIGHT_STICK_Y,
	LEFT_TRIGGER,
	RIGHT_TRIGGER,
	LEFT_STICK_LEFT,
	LEFT_STICK_RIGHT,
	LEFT_STICK_UP,
	LEFT_STICK_DOWN,
	RIGHT_STICK_LEFT,
	RIGHT_STICK_RIGHT,
	RIGHT_STICK_UP,
	RIGHT_STICK_DOWN
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

	std::unordered_map<Retry::ControllerButton, unsigned char> axes;

	cocos2d::Vec2 leftStick = cocos2d::Vec2(), rightStick = cocos2d::Vec2();
	cocos2d::Vec2 deltaLeftStick = cocos2d::Vec2(), deltaRightStick = cocos2d::Vec2();

	float leftTrigger = 0, rightTrigger = 0;
	float deltaLeftTrigger = 0, deltaRightTrigger = 0;

	float sensitivity[6]{ 0.2f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f };
};
}

};

#endif