#include "ControllerManager.h"
#include "Algorithms.h"

namespace Retry
{

XBOXController Controller::controllers[4] = { XBOXController(1), XBOXController(2), XBOXController(3), XBOXController(4) };

float Controller::deadzone[4][6] = {
		{ 0.3f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f },
		{ 0.3f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f } };

void Controller::createListener(cocos2d::Node* node)
{
	node->schedule(pollControllers, "ControllerManager");
}

void Controller::pollControllers(float delta)
{
	for (int id = 0; id < 4; id++)
	{
		controllers[id].updateState();
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
		controllers[id].getLastState()->Gamepad.wButtons ^ (1 << (int) key))
	{
		return true;
	} else return false;
}

bool Controller::isButtonUp(const ControllerButton & key, const int & id)
{
	if (controllers[id].getState()->Gamepad.wButtons ^ (1 << (int) key) &&
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
	return false;
}

bool Controller::isAxisDown(const ControllerButton & axis, const int & id)
{
	return false;
}

bool Controller::isAxisUp(const ControllerButton & axis, const int & id)
{
	return false;
}

float Controller::getAxis(const ControllerButton & axis, const int & id)
{
	return 0.0f;
}

const Vec2 Controller::getLeftStick(const int &id = 0)
{
	float xx = controllers[id].getState()->Gamepad.sThumbLX / (float) 0x7FFF;
	float yy = controllers[id].getState()->Gamepad.sThumbLY / (float) 0x7FFF;
	return Vec2(xx * (1 - deadzone[id][0]) + deadzone[id][0], yy * (1 - deadzone[id][1]) + deadzone[id][1]);
}

const Vec2 Controller::getDeltaLeftStick(const int &id = 0)
{
	float xx = controllers[id].getState()->Gamepad.sThumbLX - controllers[id].getLastState()->Gamepad.sThumbLX / (float) 0x7FFF;
	float yy = controllers[id].getState()->Gamepad.sThumbLY - controllers[id].getLastState()->Gamepad.sThumbLY / (float) 0x7FFF;
	return Vec2(xx * (1 - deadzone[id][0]) + deadzone[id][0], yy * (1 - deadzone[id][1]) + deadzone[id][1]);
}

const Vec2 Controller::getRightStick(const int &id = 0)
{
	float xx = controllers[id].getState()->Gamepad.sThumbRX / (float) 0x7FFF;
	float yy = controllers[id].getState()->Gamepad.sThumbRY / (float) 0x7FFF;
	return Vec2(xx * (1 - deadzone[id][2]) + deadzone[id][2], yy * (1 - deadzone[id][3]) + deadzone[id][3]);
}

const Vec2 Controller::getDeltaRightStick(const int &id = 0)
{
	float xx = controllers[id].getState()->Gamepad.sThumbRX - controllers[id].getLastState()->Gamepad.sThumbRX / (float) 0x7FFF;
	float yy = controllers[id].getState()->Gamepad.sThumbRY - controllers[id].getLastState()->Gamepad.sThumbRY / (float) 0x7FFF;
	return Vec2(xx * (1 - deadzone[id][2]) + deadzone[id][2], yy * (1 - deadzone[id][3]) + deadzone[id][3]);
}

const float Controller::getLeftTrigger(const int &id = 0)
{
	return controllers[id].getState()->Gamepad.bLeftTrigger / (float) 0xFF * (1 - deadzone[id][4]) + deadzone[id][4];
}

const float Controller::getRightTrigger(const int &id = 0)
{
	return controllers[id].getState()->Gamepad.bLeftTrigger / (float) 0xFF * (1 - deadzone[id][5]) + deadzone[id][5];
}

//void Controller::setDeadZone(const ControllerAxis & axis, const float & t, const int & id)
//{
//	
//}



}