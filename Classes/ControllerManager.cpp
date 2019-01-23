#include "ControllerManager.h"

namespace Retry
{

std::map<int, ControllerInfo> ControllerManager::controllers;

clock_t ControllerManager::currentTime = 0;

bool ControllerManager::useController = false;

void ControllerManager::refresh()
{
	for (auto &i : controllers)
		i.second.buttonsLast.clear();
	currentTime = clock();
	for (auto &i : controllers)
		for (auto &j : i.second.axes)
			j.second = 0;
}

void ControllerManager::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node)
{
	using namespace cocos2d;
	auto eventListener = EventListenerController::create();

	eventListener->onConnected = [](cocos2d::Controller* controller, Event* event) {
		if (controllers.find(controller->getDeviceId()) == controllers.end())
		{
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		//CCLOG("Connected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onDisconnected = [](cocos2d::Controller* controller, Event* event) {
		if (controllers.find(controller->getDeviceId()) != controllers.end())
		{
			controllers.erase(controller->getDeviceId());
		}
		//CCLOG("Disconnected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onKeyDown = [](cocos2d::Controller* controller, int key, Event* event) {
		ControllerManager::updateButton((Retry::ControllerButton) key, true, controller->getDeviceId());
		ControllerManager::setUseController(true);
	};
	eventListener->onKeyUp = [](cocos2d::Controller* controller, int key, Event* event) {
		ControllerManager::updateButton((Retry::ControllerButton) key, false, controller->getDeviceId());
		ControllerManager::setUseController(true);
	};
	eventListener->onKeyRepeat = [](cocos2d::Controller* controller, int key, Event* event) {

	};
	eventListener->onAxisEvent = [](cocos2d::Controller* controller, int key, Event* event) {
		ControllerManager::setUseController(true);
		if (controllers.find(controller->getDeviceId()) != controllers.end())
		{
			auto c = &controllers[controller->getDeviceId()];
			float newValue = controller->getKeyStatus(((EventController*) event)->getKeyCode()).value;
			switch (key)
			{
			case 0: // Left Stick X Axis
				c->deltaLeftStick.x = newValue - c->leftStick.x;
				c->leftStick.x = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->leftStick.x, c->deltaLeftStick.x, controller->getDeviceId());
				break;
			case 1: // Left Stick Y Axis
				c->deltaLeftStick.y = newValue - c->leftStick.y;
				c->leftStick.y = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->leftStick.y, c->deltaLeftStick.y, controller->getDeviceId());
				break;
			case 2: // Right Stick X Axis
				c->deltaRightStick.x = newValue - c->rightStick.x;
				c->rightStick.x = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->rightStick.x, c->deltaRightStick.x, controller->getDeviceId());
				break;
			case 3: // Right Stick Y Axis
				c->deltaRightStick.y = newValue - c->rightStick.y;
				c->rightStick.y = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->rightStick.y, c->deltaRightStick.y, controller->getDeviceId());
				break;
			case 4: // Left Trigger
				c->deltaLeftTrigger = newValue - c->leftTrigger;
				c->leftTrigger = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->leftTrigger, c->deltaLeftTrigger, controller->getDeviceId());
				break;
			case 5: // Right Trigger
				c->deltaRightTrigger = newValue - c->rightTrigger;
				c->rightTrigger = newValue;
				ControllerManager::updateAxis((Retry::ControllerButton) (key + (int) ControllerButton::AXIS_START),
													  c->rightTrigger, c->deltaRightTrigger, controller->getDeviceId());
				break;
			}
		}
	};
	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);

	// Start discovering controllers
	cocos2d::Controller::startDiscoveryController();
}

void ControllerManager::updateButton(ControllerButton key, bool isPressed, int id)
{
	auto bCurr = &controllers[id].buttons;
	if (bCurr->find(key) == bCurr->end())
	{
		if (isPressed)
			(*bCurr)[key] = currentTime;
	} else if (!isPressed)
	{
		bCurr->erase(key);
	}
	controllers[id].buttonsLast[key] = isPressed;
}

void ControllerManager::updateAxis(ControllerButton axis, float x, float dx, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto c = &controllers[id];

		auto newAxis = axis;
		switch (axis)
		{
		case ControllerButton::LEFT_STICK_X:
			newAxis = x < 0 ? ControllerButton::LEFT_STICK_LEFT : ControllerButton::LEFT_STICK_RIGHT; break;
		case ControllerButton::LEFT_STICK_Y:
			newAxis = x < 0 ? ControllerButton::LEFT_STICK_DOWN : ControllerButton::LEFT_STICK_UP; break;
		case ControllerButton::RIGHT_STICK_X:
			newAxis = x < 0 ? ControllerButton::RIGHT_STICK_LEFT : ControllerButton::RIGHT_STICK_RIGHT; break;
		case ControllerButton::RIGHT_STICK_Y:
			newAxis = x < 0 ? ControllerButton::RIGHT_STICK_DOWN : ControllerButton::RIGHT_STICK_UP; break;
		}

		if (newAxis == ControllerButton::LEFT_TRIGGER || newAxis == ControllerButton::RIGHT_TRIGGER)
		{
			if (c->axes.find(newAxis) == c->axes.end()) c->axes[newAxis];
			if (x >= c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START])                 c->axes[newAxis] |= 0b1;
			else                                                                                      c->axes[newAxis] &= ~0b1;

			if (dx != 0 && x + dx >= c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START]) c->axes[newAxis] |= 0b10;
			else                                                                                      c->axes[newAxis] &= ~0b10;

			if (dx != 0 && x + dx <= c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START]) c->axes[newAxis] |= 0b100;
			else                                                                                      c->axes[newAxis] &= ~0b100;
		} else
		{
			if (c->axes.find(newAxis) == c->axes.end()) c->axes[newAxis];
			if (abs(x) >= abs(c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START]))                 c->axes[newAxis] |= 0b1;
			else                                                                                                c->axes[newAxis] &= ~0b1;

			if (dx != 0 && abs(x + dx) >= abs(c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START])) c->axes[newAxis] |= 0b10;
			else                                                                                                c->axes[newAxis] &= ~0b10;

			if (dx != 0 && abs(x + dx) <= abs(c->sensitivity[(int) axis - (int) ControllerButton::AXIS_START])) c->axes[newAxis] |= 0b100;
			else                                                                                                c->axes[newAxis] &= ~0b100;
		}
	}
}

bool ControllerManager::isButtonPressed(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
		return controllers[id].buttons.find(key) != controllers[id].buttons.end();
	return false;
}

bool ControllerManager::isButtonDown(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return (*bLast)[key];
	}
	return false;
}

bool ControllerManager::isButtonUp(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return !(*bLast)[key];
	}
	return false;
}

float ControllerManager::buttonPressedDuration(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bCurr = &controllers[id].buttonsLast;
		if (bCurr->find(key) != bCurr->end())
			return float(currentTime - (*bCurr)[key]) / (float) CLOCKS_PER_SEC;
	}
	return 0;
}

bool ControllerManager::isAxisPressed(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b1;
}

bool ControllerManager::isAxisDown(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b10;
}

bool ControllerManager::isAxisUp(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b100;
}

cocos2d::Vec2 ControllerManager::getLStick(int id)
{
	return controllers[id].leftStick;
}

float ControllerManager::getLStickX(int id)
{
	return controllers[id].leftStick.x;
}

float ControllerManager::getLStickY(int id)
{
	return controllers[id].leftStick.y;
}

cocos2d::Vec2 ControllerManager::getDeltaLStick(int id)
{
	return controllers[id].deltaLeftStick;
}

float ControllerManager::getDeltaLStickX(int id)
{
	return controllers[id].deltaLeftStick.x;
}

float ControllerManager::getDeltaLStickY(int id)
{
	return controllers[id].deltaLeftStick.y;
}

cocos2d::Vec2  ControllerManager::getRStick(int id)
{
	return controllers[id].rightStick;
}

float ControllerManager::getRStickX(int id)
{
	return controllers[id].rightStick.x;
}

float ControllerManager::getRStickY(int id)
{
	return controllers[id].rightStick.y;
}

cocos2d::Vec2  ControllerManager::getDeltaRStick(int id)
{
	return controllers[id].deltaRightStick;
}

float ControllerManager::getDeltaRStickX(int id)
{
	return controllers[id].deltaRightStick.x;
}

float ControllerManager::getDeltaRStickY(int id)
{
	return controllers[id].deltaRightStick.y;
}

float ControllerManager::getLTrigger(int id)
{
	return controllers[id].leftTrigger;
}

float ControllerManager::getDeltaLTrigger(int id)
{
	return controllers[id].deltaLeftTrigger;
}

float ControllerManager::getRTrigger(int id)
{
	return controllers[id].rightTrigger;
}

float ControllerManager::getDeltaRTrigger(int id)
{
	return controllers[id].deltaRightTrigger;
}

void ControllerManager::setDeadZone(ControllerButton key, float t, int id)
{
	t = t < 0 ? 0 : t > 1.0f ? 1.0f : t;
	controllers[id].sensitivity[(int) key - (int) ControllerButton::AXIS_START] = t;
}

}