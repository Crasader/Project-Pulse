#include "MouseManager.h"

#include <iostream>

namespace Retry
{

cocos2d::Vec2 Mouse::position = cocos2d::Vec2(0, 0);
cocos2d::Vec2 Mouse::deltaPosition = cocos2d::Vec2(0, 0);

char Mouse::scroll = 0;

std::unordered_map<MouseButton, clock_t> Mouse::buttons;
std::unordered_map<MouseButton, bool> Mouse::buttonsLast;

clock_t Mouse::currentTime = 0;

void Mouse::refresh()
{
	buttonsLast.clear();
	currentTime = clock();
	deltaPosition.x = deltaPosition.y = 0;
}

void Mouse::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node)
{
	using namespace cocos2d;
	auto eventListener = EventListenerMouse::create();

	eventListener->onMouseDown = [](Event* event) {
		Mouse::updateButton((Retry::MouseButton) ((EventMouse*) event)->getMouseButton(), true);
	};
	eventListener->onMouseUp = [](Event* event) {
		Mouse::updateButton((Retry::MouseButton) ((EventMouse*) event)->getMouseButton(), false);
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

void Mouse::updateButton(MouseButton button, bool isPressed)
{
	if (buttons.find(button) == buttons.end())
	{
		if (isPressed)
			buttons[button] = currentTime;
	} else if (!isPressed)
	{
		buttons.erase(button);
	}
	buttonsLast[button] = isPressed;
}

bool Mouse::isButtonPressed(MouseButton button)
{
	return buttons.find(button) != buttons.end();
}

bool Mouse::isButtonDown(MouseButton button)
{
	if (buttonsLast.find(button) != buttonsLast.end())
	{
		return buttonsLast[button];
	}
	return false;
}

bool Mouse::isButtonUp(MouseButton button)
{
	if (buttonsLast.find(button) != buttonsLast.end())
	{
		return !buttonsLast[button];
	}
	return false;
}

float Mouse::buttonPressedDuration(MouseButton button)
{
	if (buttons.find(button) != buttons.end())
		return float(currentTime - buttons[button]) / (float) CLOCKS_PER_SEC;
	return 0;
}

}