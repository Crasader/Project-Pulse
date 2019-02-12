#include "ControllerManager.h"
#include "Algorithms.h"

#include "GameSettings.h"

namespace Retry
{

XBOXController Controller::controllers[4] = { XBOXController(1), XBOXController(2), XBOXController(3), XBOXController(4) };

Vec2 Controller::vibration[4][2] = {
	{ Vec2::ZERO, Vec2::ZERO },
	{ Vec2::ZERO, Vec2::ZERO },
	{ Vec2::ZERO, Vec2::ZERO },
	{ Vec2::ZERO, Vec2::ZERO }, };

float Controller::deadzone[4][4] = {
		{ 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.0f, 0.0f } };

void Controller::createListener(cocos2d::Node* node)
{
	node->schedule(pollControllers, "ControllerManager");
}

void Controller::pollControllers(float delta)
{
	for (int id = 0; id < 4; id++)
	{
		controllers[id].updateState();
		updateVibration(delta, id);
	}
}

bool Controller::isButtonPressed(const ControllerButton & key, const int & id)
{
	if (controllers[id].getState()->Gamepad.wButtons & (1 << (int) key))
	{
		return true;
	} else return false;
}

bool Controller::isButtonDown(const ControllerButton & key, const int & id)
{
	if (controllers[id].getState()->Gamepad.wButtons & (1 << (int) key) &&
		!(controllers[id].getLastState()->Gamepad.wButtons & (1 << (int) key)))
	{
		return true;
	} else return false;
}

bool Controller::isButtonUp(const ControllerButton & key, const int & id)
{
	if (!(controllers[id].getState()->Gamepad.wButtons & (1 << (int) key)) &&
		controllers[id].getLastState()->Gamepad.wButtons & (1 << (int) key))
	{
		return true;
	} else return false;
}

//float Controller::buttonPressedDuration(const ControllerButton & key, const int & id)
//{
//	return 0.0f;
//}

bool Controller::isAxisPressed(const ControllerButton & axis, const int & id)
{
	float value = getAxis(axis, id);
	if (axis == ControllerButton::LEFT_STICK_X ||
		axis == ControllerButton::LEFT_STICK_Y ||
		axis == ControllerButton::RIGHT_STICK_X ||
		axis == ControllerButton::RIGHT_STICK_Y)
	{
		return value != 0;
	}
	if (axis == ControllerButton::LEFT_STICK_LEFT ||
		axis == ControllerButton::LEFT_STICK_DOWN ||
		axis == ControllerButton::RIGHT_STICK_LEFT ||
		axis == ControllerButton::RIGHT_STICK_DOWN)
	{
		return value < 0;
	} else return value > 0;
}

bool Controller::isAxisDown(const ControllerButton & axis, const int & id)
{
	float value = getAxis(axis, id);
	float dValue = getAxis(axis, id, true);
	if (axis == ControllerButton::LEFT_STICK_X ||
		axis == ControllerButton::LEFT_STICK_Y ||
		axis == ControllerButton::RIGHT_STICK_X ||
		axis == ControllerButton::RIGHT_STICK_Y)
	{
		return value != value == dValue && value != 0;
	}
	if (axis == ControllerButton::LEFT_STICK_LEFT ||
		axis == ControllerButton::LEFT_STICK_DOWN ||
		axis == ControllerButton::RIGHT_STICK_LEFT ||
		axis == ControllerButton::RIGHT_STICK_DOWN)
	{
		return value == dValue && value < 0;
	} else return value == dValue && value > 0;

	//return getAxis(axis, id) - getAxis(axis, id, true) < 0.001f;
}

bool Controller::isAxisUp(const ControllerButton & axis, const int & id)
{
	float value = getAxis(axis, id);
	float dValue = getAxis(axis, id, true);
	if (axis == ControllerButton::LEFT_STICK_X ||
		axis == ControllerButton::LEFT_STICK_Y ||
		axis == ControllerButton::RIGHT_STICK_X ||
		axis == ControllerButton::RIGHT_STICK_Y)
	{
		return value == 0 && dValue != 0;;
	}
	if (axis == ControllerButton::LEFT_STICK_LEFT ||
		axis == ControllerButton::LEFT_STICK_DOWN ||
		axis == ControllerButton::RIGHT_STICK_LEFT ||
		axis == ControllerButton::RIGHT_STICK_DOWN)
	{
		return value == 0 && dValue > 0;
	} else return value == 0 && dValue < 0;
}

float Controller::getAxis(const ControllerButton & axis, const int & id, const bool &isDelta)
{
	switch (axis)
	{
	case ControllerButton::LEFT_STICK_LEFT:
	case ControllerButton::LEFT_STICK_RIGHT:
	case ControllerButton::LEFT_STICK_X:
		return isDelta ? getDeltaLeftStick().x : getLeftStick().x;
	case ControllerButton::LEFT_STICK_UP:
	case ControllerButton::LEFT_STICK_DOWN:
	case ControllerButton::LEFT_STICK_Y:
		return isDelta ? getDeltaLeftStick().y : getLeftStick().y;
	case ControllerButton::RIGHT_STICK_LEFT:
	case ControllerButton::RIGHT_STICK_RIGHT:
	case ControllerButton::RIGHT_STICK_X:
		return isDelta ? getDeltaRightStick().x : getRightStick().x;
	case ControllerButton::RIGHT_STICK_UP:
	case ControllerButton::RIGHT_STICK_DOWN:
	case ControllerButton::RIGHT_STICK_Y:
		return isDelta ? getDeltaRightStick().y : getRightStick().y;
	case ControllerButton::LEFT_TRIGGER:
		return isDelta ? getDeltaLeftTrigger() : getLeftTrigger();
	case ControllerButton::RIGHT_TRIGGER:
		return isDelta ? getDeltaRightTrigger() : getRightTrigger();
	default:
		throw std::logic_error("Unexpected Button Request");
		return false;
	}
}

const Vec2 Controller::getLeftStick(const int &id)
{
	Vec2 input = Vec2(controllers[id].getState()->Gamepad.sThumbLX,
					  controllers[id].getState()->Gamepad.sThumbLY) / (float) 0x7FFF;

	//return radialDeadzone(input, deadzone[id][0]);
	//return squareDeadzone(input, deadzone[id][0]);
	//return crossDeadzone(input, deadzone[id][0]);
	return radialDeadzone(crossDeadzone(input, deadzone[id][0]), deadzone[id][0]);
}

const Vec2 Controller::getDeltaLeftStick(const int &id)
{
	Vec2 input = Vec2(controllers[id].getState()->Gamepad.sThumbLX - controllers[id].getLastState()->Gamepad.sThumbLX,
					  controllers[id].getState()->Gamepad.sThumbLY - controllers[id].getLastState()->Gamepad.sThumbLY) / (float) 0x7FFF;

	//return radialDeadzone(input, deadzone[id][0]);
	//return squareDeadzone(input, deadzone[id][0]);
	//return crossDeadzone(input, deadzone[id][0]);
	return radialDeadzone(crossDeadzone(input, deadzone[id][0]), deadzone[id][0]);
}

const Vec2 Controller::getRightStick(const int &id)
{
	Vec2 input = Vec2(controllers[id].getState()->Gamepad.sThumbRX / (float) 0x7FFF,
					  controllers[id].getState()->Gamepad.sThumbRY / (float) 0x7FFF);

	//return radialDeadzone(input, deadzone[id][1]);
	//return squareDeadzone(input, deadzone[id][1]);
	//return crossDeadzone(input, deadzone[id][0]);
	return radialDeadzone(crossDeadzone(input, deadzone[id][1]), deadzone[id][1]);
}

const Vec2 Controller::getDeltaRightStick(const int &id)
{
	Vec2 input = Vec2(controllers[id].getState()->Gamepad.sThumbRX - controllers[id].getLastState()->Gamepad.sThumbRX,
					  controllers[id].getState()->Gamepad.sThumbRY - controllers[id].getLastState()->Gamepad.sThumbRY) / (float) 0x7FFF;

	//return radialDeadzone(input, deadzone[id][1]);
	//return squareDeadzone(input, deadzone[id][1]);
	//return crossDeadzone(input, deadzone[id][1]);
	return radialDeadzone(crossDeadzone(input, deadzone[id][1]), deadzone[id][1]);
}

const float Controller::getLeftTrigger(const int &id)
{
	float input = controllers[id].getState()->Gamepad.bLeftTrigger / (float) 0xFF;
	if (input < deadzone[id][2])
	{
		return 0.0f;
	} else
	{
		return (input - deadzone[id][2]) / (1 - deadzone[id][2]);
	}
}

const float Controller::getDeltaLeftTrigger(const int &id)
{
	float input = (controllers[id].getState()->Gamepad.bLeftTrigger -
				   controllers[id].getLastState()->Gamepad.bLeftTrigger) / (float) 0xFF;
	if (input < deadzone[id][2])
	{
		return 0.0f;
	} else
	{
		return (input - deadzone[id][2]) / (1 - deadzone[id][2]);
	}
}

const float Controller::getRightTrigger(const int &id)
{
	float input = controllers->getState()->Gamepad.bRightTrigger / (float) 0xFF;
	if (input < deadzone[id][3])
	{
		return 0.0f;
	} else
	{
		return (input - deadzone[id][3]) / (1 - deadzone[id][3]);
	}
}

const float Controller::getDeltaRightTrigger(const int &id)
{
	float input = (controllers[id].getState()->Gamepad.bRightTrigger -
				   controllers[id].getLastState()->Gamepad.bRightTrigger) / (float) 0xFF;
	if (input < deadzone[id][3])
	{
		return 0.0f;
	} else
	{
		return (input - deadzone[id][3]) / (1 - deadzone[id][3]);
	}
}

const Vec2 Controller::radialDeadzone(const Vec2 & rawInput, const float & deadzone)
{
	if (rawInput.getLengthSq() < deadzone * deadzone)
	{
		return Vec2::ZERO;
	} else
	{
		return rawInput.getNormalized() * (rawInput.getLength() - deadzone) / (1 - deadzone);
	}
}

// @ Not yet implemented
const Vec2 Controller::squareDeadzone(const Vec2 & rawInput, const float & deadzone)
{
	return rawInput;
}

const Vec2 Controller::crossDeadzone(const Vec2 & rawInput, const float & deadzone)
{
	Vec2 newInput = rawInput;
	if (abs(rawInput.x) < deadzone)
	{
		newInput.x = 0;
	}
	if (abs(rawInput.y) < deadzone)
	{
		newInput.y = 0;
	}
	return newInput;
}

void Controller::updateVibration(const float &delta, const int &id)
{
	bool doStop = false;

	if (vibration[id][0].x < 0)
	{
		vibration[id][0].x = vibration[id][0].y = 0;

		doStop = true;

		//controllers[id].Vibrate(0, vibration[id][1].y);
	}

	if (vibration[id][1].x < 0)
	{
		vibration[id][1].x = vibration[id][1].y = 0;

		doStop = true;

		//controllers[id].Vibrate(vibration[id][0].y, 0);
	}

	if (doStop)
	{
		controllers[id].Vibrate(vibration[id][0].y, vibration[id][1].y);
	}

	if (vibration[id][0].x != 0)
		vibration[id][0].x -= delta;
	if (vibration[id][1].x != 0)
		vibration[id][1].x -= delta;
	//static float count = 0;
	//if (count > 3)
	//{
	//	count = 0;
	//	controllers[id].Vibrate(vibration[id][0].y, vibration[id][1].y);
	//}
	//count += delta;
}

void Controller::vibrate(const float &intensity, const float &duration, const int &id)
{
	if (!Config::doVibration()) return;

	float newIntensity = clamp(intensity, 0, 1);
	float newDuration = clamp(duration, 0, duration);
	vibration[id][0] = Vec2(newDuration, newIntensity);
	vibration[id][1] = Vec2(newDuration, newIntensity);
	controllers[id].Vibrate(newIntensity, newIntensity);
}

void Controller::vibrateLeft(const float &intensity, const float &duration, const int &id)
{
	if (!Config::doVibration()) return;

	float newIntensity = clamp(intensity, 0, 1);
	float newDuration = clamp(duration, 0, duration);
	vibration[id][0] = Vec2(newDuration, newIntensity);
	controllers[id].Vibrate(newIntensity, vibration[id][1].y);
}

void Controller::vibrateRight(const float &intensity, const float &duration, const int &id)
{
	if (!Config::doVibration()) return;

	float newIntensity = clamp(intensity, 0, 1);
	float newDuration = clamp(duration, 0, duration);
	vibration[id][1] = Vec2(newDuration, newIntensity);
	controllers[id].Vibrate(vibration[id][0].y, newIntensity);
}

}