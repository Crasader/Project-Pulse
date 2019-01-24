#include "ControllerManager.h"

namespace { int keyCodeConvert(int codeToConvert); }

namespace Retry
{

std::map<int, ControllerInfo> Controller::controllers;

clock_t Controller::currentTime = 0;

bool Controller::useController = false;

void Controller::refresh(float delta)
{
	for (auto &i : controllers)
	{
		i.second.buttonsLast.clear();
		for (auto &j : i.second.axes)
			j.second = 0;
	}
	currentTime = clock();		
}

void Controller::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node)
{
	auto eventListener = cocos2d::EventListenerController::create();

	eventListener->onConnected = [](cocos2d::Controller* controller, cocos2d::Event* event) {
		if (controllers.find(controller->getDeviceId()) == controllers.end())
		{
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		CCLOG("Connected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onDisconnected = [](cocos2d::Controller* controller, cocos2d::Event* event) {
		if (controllers.find(controller->getDeviceId()) != controllers.end())
		{
			controllers.erase(controller->getDeviceId());
		}
		CCLOG("Disconnected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onKeyDown = [](cocos2d::Controller* controller, int key, cocos2d::Event* event) {
		key = keyCodeConvert(key);
		if (controllers.find(controller->getDeviceId()) == controllers.end())
		{
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		Controller::updateButton((Retry::ControllerButton) key, true, controller->getDeviceId());
		Controller::setUseController(true);
	};
	eventListener->onKeyUp = [](cocos2d::Controller* controller, int key, cocos2d::Event* event) {
		key = keyCodeConvert(key);
		if (controllers.find(controller->getDeviceId()) == controllers.end())
		{
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		Controller::updateButton((Retry::ControllerButton) key, false, controller->getDeviceId());
		Controller::setUseController(true);
	};
	eventListener->onKeyRepeat = [](cocos2d::Controller* controller, int key, cocos2d::Event* event) {

	};
	eventListener->onAxisEvent = [](cocos2d::Controller* controller, int key, cocos2d::Event* event) {
		if (controllers.find(controller->getDeviceId()) == controllers.end())
		{
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		Controller::setUseController(true);
		if (controllers.find(controller->getDeviceId()) != controllers.end())
		{
			auto c = &controllers[controller->getDeviceId()];
			float newValue = controller->getKeyStatus(((cocos2d::EventController*) event)->getKeyCode()).value;
			key = keyCodeConvert(key);
			switch ((ControllerButton) key)
			{
			case ControllerButton::LEFT_STICK_X: // Left Stick X Axis
			case (ControllerButton) 0:
				c->deltaLeftStick.x = newValue - c->leftStick.x;
				c->leftStick.x = newValue;
				Controller::updateAxis(ControllerButton::LEFT_STICK_X, c->leftStick.x, c->deltaLeftStick.x, controller->getDeviceId());
				break;
			case ControllerButton::LEFT_STICK_Y: // Left Stick Y Axis
			case (ControllerButton) 1:
				c->deltaLeftStick.y = newValue - c->leftStick.y;
				c->leftStick.y = newValue;
				Controller::updateAxis(ControllerButton::LEFT_STICK_Y, c->leftStick.y, c->deltaLeftStick.y, controller->getDeviceId());
				break;
			case Retry::ControllerButton::RIGHT_STICK_X: // Right Stick X Axis
			case (ControllerButton) 2:
				c->deltaRightStick.x = newValue - c->rightStick.x;
				c->rightStick.x = newValue;
				Controller::updateAxis(ControllerButton::RIGHT_STICK_X, c->rightStick.x, c->deltaRightStick.x, controller->getDeviceId());
				break;
			case ControllerButton::RIGHT_STICK_Y: // Right Stick Y Axis
			case (ControllerButton) 3:
				c->deltaRightStick.y = newValue - c->rightStick.y;
				c->rightStick.y = newValue;
				Controller::updateAxis(ControllerButton::RIGHT_STICK_X, c->rightStick.y, c->deltaRightStick.y, controller->getDeviceId());
				break;
			case ControllerButton::LEFT_TRIGGER: // Left Trigger
			case (ControllerButton) 4:
				c->deltaLeftTrigger = newValue - c->leftTrigger;
				c->leftTrigger = newValue;
				Controller::updateAxis(ControllerButton::LEFT_TRIGGER, c->leftTrigger, c->deltaLeftTrigger, controller->getDeviceId());
				break;
			case ControllerButton::RIGHT_TRIGGER: // Right Trigger
			case (ControllerButton) 5:
				c->deltaRightTrigger = newValue - c->rightTrigger;
				c->rightTrigger = newValue;
				Controller::updateAxis(ControllerButton::RIGHT_TRIGGER, c->rightTrigger, c->deltaRightTrigger, controller->getDeviceId());
				break;
			}
		}
	};
	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);

	cocos2d::Controller::startDiscoveryController();

	//cocos2d::Controller::

	node->schedule(refresh, "ControllerManager");
}

void Controller::updateButton(ControllerButton key, bool isPressed, int id)
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

void Controller::updateAxis(ControllerButton axis, float x, float dx, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto c = &controllers[id];

		auto newAxis = axis;
		switch ( axis)
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

bool Controller::isButtonPressed(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
		return controllers[id].buttons.find(key) != controllers[id].buttons.end();
	return false;
}

bool Controller::isButtonDown(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return (*bLast)[key];
	}
	return false;
}

bool Controller::isButtonUp(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return !(*bLast)[key];
	}
	return false;
}

float Controller::buttonPressedDuration(ControllerButton key, int id)
{
	if (controllers.find(id) != controllers.end())
	{
		auto bCurr = &controllers[id].buttonsLast;
		if (bCurr->find(key) != bCurr->end())
			return float(currentTime - (*bCurr)[key]) / (float) CLOCKS_PER_SEC;
	}
	return 0;
}

bool Controller::isAxisPressed(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b1;
}

bool Controller::isAxisDown(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b10;
}

bool Controller::isAxisUp(ControllerButton axis, int id)
{
	return controllers[id].axes[axis] & 0b100;
}

cocos2d::Vec2 Controller::getLStick(int id)
{
	return controllers[id].leftStick;
}

float Controller::getLStickX(int id)
{
	return controllers[id].leftStick.x;
}

float Controller::getLStickY(int id)
{
	return controllers[id].leftStick.y;
}

cocos2d::Vec2 Controller::getDeltaLStick(int id)
{
	return controllers[id].deltaLeftStick;
}

float Controller::getDeltaLStickX(int id)
{
	return controllers[id].deltaLeftStick.x;
}

float Controller::getDeltaLStickY(int id)
{
	return controllers[id].deltaLeftStick.y;
}

cocos2d::Vec2  Controller::getRStick(int id)
{
	return controllers[id].rightStick;
}

float Controller::getRStickX(int id)
{
	return controllers[id].rightStick.x;
}

float Controller::getRStickY(int id)
{
	return controllers[id].rightStick.y;
}

cocos2d::Vec2  Controller::getDeltaRStick(int id)
{
	return controllers[id].deltaRightStick;
}

float Controller::getDeltaRStickX(int id)
{
	return controllers[id].deltaRightStick.x;
}

float Controller::getDeltaRStickY(int id)
{
	return controllers[id].deltaRightStick.y;
}

float Controller::getLTrigger(int id)
{
	return controllers[id].leftTrigger;
}

float Controller::getDeltaLTrigger(int id)
{
	return controllers[id].deltaLeftTrigger;
}

float Controller::getRTrigger(int id)
{
	return controllers[id].rightTrigger;
}

float Controller::getDeltaRTrigger(int id)
{
	return controllers[id].deltaRightTrigger;
}

void Controller::setDeadZone(ControllerButton key, float t, int id)
{
	t = t < 0 ? 0 : t > 1.0f ? 1.0f : t;
	controllers[id].sensitivity[(int) key - (int) ControllerButton::AXIS_START] = t;
}

}

namespace
{

int keyCodeConvert(int codeToConvert)
{
	switch (codeToConvert)
	{
	case cocos2d::Controller::Key::BUTTON_A:
		return (int) Retry::ControllerButton::A;
	case cocos2d::Controller::Key::BUTTON_B:
		return (int) Retry::ControllerButton::B;
	case cocos2d::Controller::Key::BUTTON_X:
		return (int) Retry::ControllerButton::X;
	case cocos2d::Controller::Key::BUTTON_Y:
		return (int) Retry::ControllerButton::Y;
	case cocos2d::Controller::Key::BUTTON_SELECT:
		return (int) Retry::ControllerButton::SELECT;
	case cocos2d::Controller::Key::BUTTON_START:
		return (int) Retry::ControllerButton::START;
	case cocos2d::Controller::Key::BUTTON_DPAD_LEFT:
		return (int) Retry::ControllerButton::DPAD_LEFT;
	case cocos2d::Controller::Key::BUTTON_DPAD_RIGHT:
		return (int) Retry::ControllerButton::DPAD_RIGHT;
	case cocos2d::Controller::Key::BUTTON_DPAD_DOWN:
		return (int) Retry::ControllerButton::DPAD_DOWN;
	case cocos2d::Controller::Key::BUTTON_DPAD_UP:
		return (int) Retry::ControllerButton::DPAD_UP;
	case cocos2d::Controller::Key::BUTTON_LEFT_SHOULDER:
		return (int) Retry::ControllerButton::LEFT_BUMPER;
	case cocos2d::Controller::Key::BUTTON_RIGHT_SHOULDER:
		return (int) Retry::ControllerButton::RIGHT_BUMPER;
	case cocos2d::Controller::Key::BUTTON_LEFT_THUMBSTICK:
		return (int) Retry::ControllerButton::LEFT_STICK;
	case cocos2d::Controller::Key::BUTTON_RIGHT_THUMBSTICK:
		return (int) Retry::ControllerButton::RIGHT_STICK;
	case cocos2d::Controller::Key::AXIS_LEFT_TRIGGER:
		return (int) Retry::ControllerButton::LEFT_TRIGGER;
	case cocos2d::Controller::Key::AXIS_RIGHT_TRIGGER:
		return (int) Retry::ControllerButton::RIGHT_TRIGGER;
	case cocos2d::Controller::Key::JOYSTICK_LEFT_X:
		return (int) Retry::ControllerButton::LEFT_STICK_X;
	case cocos2d::Controller::Key::JOYSTICK_LEFT_Y:
		return (int) Retry::ControllerButton::LEFT_STICK_Y;
	case cocos2d::Controller::Key::JOYSTICK_RIGHT_X:
		return (int) Retry::ControllerButton::RIGHT_STICK_X; 
	case cocos2d::Controller::Key::JOYSTICK_RIGHT_Y:
		return (int) Retry::ControllerButton::RIGHT_STICK_Y;
	}
	return codeToConvert;
}

}