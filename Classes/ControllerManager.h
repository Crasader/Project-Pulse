#ifndef CONTROLLER_MANAGER
#define CONTROLLER_MANAGER

#include "cocos2d.h"

#include "CXBOXController.h"

#include <ctime>
#include <unordered_map>

using cocos2d::Vec2;


namespace Retry
{

namespace
{
struct ControllerInfo;
};

enum class ControllerButton;

class Controller abstract
{
public:
	static void createListener(cocos2d::Node* node);

	static void pollControllers(float delta);

	static bool isButtonPressed(const ControllerButton &key, const int &id = 0);
	static bool isButtonDown(const ControllerButton &key, const int &id = 0);
	static bool isButtonUp(const ControllerButton &key, const int &id = 0);

	//static float buttonPressedDuration(const ControllerButton &key, const int &id = 0);

	static bool isAxisPressed(const ControllerButton &axis, const int &id = 0);
	static bool isAxisDown(const ControllerButton &axis, const int &id = 0);
	static bool isAxisUp(const ControllerButton &axis, const int &id = 0);

	static float getAxis(const ControllerButton &axis, const int &id = 0);

	static const Vec2 getLeftStick(const int &id = 0);
	static const Vec2 getDeltaLeftStick(const int &id = 0);

	static const Vec2 getRightStick(const int &id = 0);
	static const Vec2 getDeltaRightStick(const int &id = 0);

	static const float getLeftTrigger(const int &id = 0);
	static const float getRightTrigger(const int &id = 0);

	//static void setDeadZone(const ControllerAxis &axis, const float &t, const int &id = 0);

private:
	static XBOXController controllers[4];

	static float deadzone[4][6];

	//static clock_t currentTime;

	//static std::unordered_map<ControllerButton, clock_t> times[4];
};

enum class ControllerButton
{
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	START,
	BACK,
	LEFT_STICK,
	RIGHT_STICK,
	LEFT_BUMPER,
	RIGHT_BUMPER,
	A = 12,
	B,
	X,
	Y,
	LEFT_TRIGGER,
	RIGHT_TRIGGER,
	LEFT_STICK_X,
	LEFT_STICK_Y,
	RIGHT_STICK_X,
	RIGHT_STICK_Y
};

namespace
{
struct ControllerInfo
{
	float sensitivity[6] { 0.3f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f };
};
}

};

#endif