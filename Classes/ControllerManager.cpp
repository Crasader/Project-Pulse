#include "ControllerManager.h"

using namespace Retry;

ControllerManager* ControllerManager::instance = 0;

std::map<int, ControllerInfo> ControllerManager::controllers;

clock_t ControllerManager::currentTime = 0;

ControllerManager* ControllerManager::getInstance() {
	return instance == 0 ? (instance = new ControllerManager()) : instance;
}

void ControllerManager::refresh() {
	for (auto &i : controllers)
		i.second.buttonsLast.clear();
	currentTime = clock();
}

void ControllerManager::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node) {
	using namespace cocos2d;
	auto eventListener = EventListenerController::create();

	eventListener->onConnected = [](cocos2d::Controller* controller, Event* event) {
		if (controllers.find(controller->getDeviceId()) == controllers.end()) {
			controllers[controller->getDeviceId()] = ControllerInfo(controller->getDeviceId(), controller->getDeviceName());
		}
		//CCLOG("Connected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onDisconnected = [](cocos2d::Controller* controller, Event* event) {
		if (controllers.find(controller->getDeviceId()) != controllers.end()) {
			controllers.erase(controller->getDeviceId());
		}
		//CCLOG("Disconnected controller: %s, With ID: %d", controller->getDeviceName().c_str(), controller->getDeviceId());
	};
	eventListener->onKeyDown = [](cocos2d::Controller* controller, int key, Event* event) {
		ControllerManager::getInstance()->updateButton((Retry::ControllerButton) key, true, controller->getDeviceId());
	};
	eventListener->onKeyUp = [](cocos2d::Controller* controller, int key, Event* event) {
		ControllerManager::getInstance()->updateButton((Retry::ControllerButton) key, false, controller->getDeviceId());
	};
	eventListener->onKeyRepeat = [](cocos2d::Controller* controller, int key, Event* event) {

	};
	eventListener->onAxisEvent = [](cocos2d::Controller* controller, int key, Event* event) {
		if (controllers.find(controller->getDeviceId()) != controllers.end()) {
			auto c = &controllers[controller->getDeviceId()];
			float newValue = controller->getKeyStatus(((EventController*) event)->getKeyCode()).value;
			switch (key) {
				case 0: // Left Stick X Axis
					c->deltaLeftStick.x = newValue - c->leftStick.x;
					c->leftStick.x = newValue;
					break;
				case 1: // Left Stick Y Axis
					c->deltaLeftStick.y = newValue - c->leftStick.y;
					c->leftStick.y = newValue;
					break;
				case 2: // Right Stick X Axis
					c->deltaRightStick.x = newValue - c->rightStick.x;
					c->rightStick.x = newValue;
					break;
				case 3: // Right Stick Y Axis
					c->deltaRightStick.y = newValue - c->rightStick.y;
					c->rightStick.y = newValue;
					break;
				case 4: // Left Trigger
					c->deltaLeftTrigger = newValue - c->deltaLeftTrigger;
					c->leftTrigger = newValue;
					break;
				case 5: // Right Trigger
					c->deltaRightTrigger = newValue - c->deltaRightTrigger;
					c->rightTrigger = newValue;
					break;
			}
		}
	};
	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);

	// Start discovering controllers
	Controller::startDiscoveryController();
}

void ControllerManager::updateButton(ControllerButton key, bool isPressed, int id) {
	auto bCurr = &controllers[id].buttons;
	if (bCurr->find(key) == bCurr->end()) {
		if (isPressed)
			(*bCurr)[key] = currentTime;
	} else if (!isPressed) {
		bCurr->erase(key);
	}
	controllers[id].buttonsLast[key] = isPressed;
}

bool ControllerManager::isButtonPressed(ControllerButton key, int id) {
	if (controllers.find(id) != controllers.end())
		return controllers[id].buttons.find(key) != controllers[id].buttons.end();
	return false;
}

bool ControllerManager::isButtonDown(ControllerButton key, int id) {
	if (controllers.find(id) != controllers.end()) {
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return (*bLast)[key];
	}
	return false;
}

bool ControllerManager::isButtonUp(ControllerButton key, int id) {
	if (controllers.find(id) != controllers.end()) {
		auto bLast = &controllers[id].buttonsLast;
		if (bLast->find(key) != bLast->end())
			return !(*bLast)[key];
	}
	return false;
}

float ControllerManager::buttonPressedDuration(ControllerButton key, int id) {
	if (controllers.find(id) != controllers.end()) {
		auto bCurr = &controllers[id].buttonsLast;
		if (bCurr->find(key) != bCurr->end())
			return float(currentTime - (*bCurr)[key]) / (float) CLOCKS_PER_SEC;
	}
	return 0;
}

cocos2d::Vec2 ControllerManager::getLStick(int id) {
	return controllers[id].leftStick;
}

float ControllerManager::getLStickX(int id) {
	return controllers[id].leftStick.x;
}

float ControllerManager::getLStickY(int id) {
	return controllers[id].leftStick.y;
}

cocos2d::Vec2 ControllerManager::getDeltaLStick(int id) {
	return controllers[id].deltaLeftStick;
}

float ControllerManager::getDeltaLStickX(int id) {
	return controllers[id].deltaLeftStick.x;
}

float ControllerManager::getDeltaLStickY(int id) {
	return controllers[id].deltaLeftStick.y;
}

cocos2d::Vec2  ControllerManager::getRStick(int id) {
	return controllers[id].rightStick;
}

float ControllerManager::getRStickX(int id) {
	return controllers[id].rightStick.x;
}

float ControllerManager::getRStickY(int id) {
	return controllers[id].rightStick.y;
}

cocos2d::Vec2  ControllerManager::getDeltaRStick(int id) {
	return controllers[id].deltaRightStick;
}

float ControllerManager::getDeltaRStickX(int id) {
	return controllers[id].deltaRightStick.x;
}

float ControllerManager::getDeltaRStickY(int id) {
	return controllers[id].deltaRightStick.y;
}

float ControllerManager::getLTrigger(int id) {
	return controllers[id].leftTrigger;
}

float ControllerManager::getDeltaLTrigger(int id) {
	return controllers[id].deltaLeftTrigger;
}

float ControllerManager::getRTrigger(int id) {
	return controllers[id].rightTrigger;
}

float ControllerManager::getDeltaRTrigger(int id) {
	return controllers[id].deltaRightTrigger;
}