#include "KeyboardManager.h"
#include "ControllerManager.h"

namespace Retry
{

clock_t Keyboard::currentTime = 0;

std::unordered_map<KeyCode, clock_t> Keyboard::keys;
std::unordered_map<KeyCode, bool> Keyboard::keysLast;

void Keyboard::refresh()
{
	keysLast.clear();
	currentTime = clock();
}

void Keyboard::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node)
{
	using namespace cocos2d;
	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Retry::Keyboard::updateKey((Retry::KeyCode) keyCode, true);
		Retry::ControllerManager::setUseController(false);
	};
	eventListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Retry::Keyboard::updateKey((Retry::KeyCode) keyCode, false);
		Retry::ControllerManager::setUseController(false);
	};
	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);
}

void Keyboard::updateKey(KeyCode key, bool isPressed)
{
	if (keys.find(key) == keys.end())
	{
		if (isPressed)
			keys[key] = currentTime;
	} else if (!isPressed)
	{
		keys.erase(key);
	}
	keysLast[key] = isPressed;
}

bool Keyboard::isKeyPressed(KeyCode key)
{
	return keys.find(key) != keys.end();
}

bool Keyboard::isKeyDown(KeyCode key)
{
	if (keysLast.find(key) != keysLast.end())
	{
		return keysLast[key];
	}
	return false;
}

bool Keyboard::isKeyUp(KeyCode key)
{
	if (keysLast.find(key) != keysLast.end())
	{
		return !keysLast[key];
	}
	return false;
}

float Keyboard::keyPressedDuration(KeyCode key)
{
	if (keys.find(key) != keys.end())
		return float(currentTime - keys[key]) / (float) CLOCKS_PER_SEC;
	return 0;
}
}