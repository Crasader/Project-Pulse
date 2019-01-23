#include "MouseManager.h"

#include <iostream>

using namespace Retry;

MouseManager* MouseManager::instance = 0;

cocos2d::Vec2 MouseManager::position = cocos2d::Vec2(0, 0);
cocos2d::Vec2 MouseManager::deltaPosition = cocos2d::Vec2(0, 0);

char MouseManager::scroll = 0;

std::unordered_map<MouseButton, clock_t> MouseManager::buttons;
std::unordered_map<MouseButton, bool> MouseManager::buttonsLast;

clock_t MouseManager::currentTime = 0;

MouseManager* MouseManager::getInstance() {
	return instance == 0 ? (instance = new MouseManager()) : instance;
}

void MouseManager::refresh() {
	buttonsLast.clear();
	currentTime = clock();
	deltaPosition.x = deltaPosition.y = 0;
}

void MouseManager::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node) {
	using namespace cocos2d;
	auto eventListener = EventListenerMouse::create();

	eventListener->onMouseDown = [](Event* event) {
		MouseManager::getInstance()->updateButton((Retry::MouseButton) ((EventMouse*) event)->getMouseButton(), true);
	};
	eventListener->onMouseUp = [](Event* event) {
		MouseManager::getInstance()->updateButton((Retry::MouseButton) ((EventMouse*) event)->getMouseButton(), false);
	};
	eventListener->onMouseMove = [](Event* event) {
		EventMouse* e = (EventMouse*) event;
		deltaPosition.x = e->getCursorX() - position.x;
		deltaPosition.y = e->getCursorY() - position.y;
		position.x = e->getCursorX();
		position.y = e->getCursorY();
	};
	eventListener->onMouseScroll = [](Event* event) {
		scroll = ((EventMouse*) event)->getScrollY();
	};

	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);
	//return eventListener;
}

void MouseManager::updateButton(MouseButton button, bool isPressed) {
	if (buttons.find(button) == buttons.end()) {
		if (isPressed)
			buttons[button] = currentTime;
	} else if (!isPressed) {
		buttons.erase(button);
	}
	buttonsLast[button] = isPressed;
}

bool MouseManager::isButtonPressed(MouseButton button) {
	return buttons.find(button) != buttons.end();
}

bool MouseManager::isButtonDown(MouseButton button) {
	if (buttonsLast.find(button) != buttonsLast.end()) {
		return buttonsLast[button];
	}
	return false;
}

bool MouseManager::isButtonUp(MouseButton button) {
	if (buttonsLast.find(button) != buttonsLast.end()) {
		return !buttonsLast[button];
	}
	return false;
}

float MouseManager::buttonPressedDuration(MouseButton button) {
	if (buttons.find(button) != buttons.end())
		return float(currentTime - buttons[button]) / (float) CLOCKS_PER_SEC;
	return 0;
}
