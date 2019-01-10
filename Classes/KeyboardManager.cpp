#include "KeyboardManager.h"

KeyboardManager* KeyboardManager::instance = 0;

std::map<cocos2d::EventKeyboard::KeyCode, clock_t> KeyboardManager::keys;
std::map<cocos2d::EventKeyboard::KeyCode, bool> KeyboardManager::keysLast;

KeyboardManager* KeyboardManager::getInstance() {
	return instance == 0 ? (instance = new KeyboardManager()) : instance;
}

cocos2d::EventListenerKeyboard* KeyboardManager::createListener() {
	using namespace cocos2d;
	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		KeyboardManager::getInstance()->updateKey(keyCode, true);
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		KeyboardManager::getInstance()->updateKey(keyCode, false);
	};
	return eventListener;
}

void KeyboardManager::updateKey(cocos2d::EventKeyboard::KeyCode key, bool isPressed) {
	if (isPressed && keys.find(key) == keys.end()) {
		keys[key] = clock();
		keysLast[key] = 0;
	} else {
		keys.erase(key);
		keysLast[key] = 1;
	}
}

bool KeyboardManager::isKeyPressed(cocos2d::EventKeyboard::KeyCode key) {
	return keys.find(key) != keys.end();
}

bool KeyboardManager::isKeyDown(cocos2d::EventKeyboard::KeyCode key) {
	if (keysLast.find(key) != keysLast.end())
		return !keysLast[key];
	return false;
}

bool KeyboardManager::isKeyUp(cocos2d::EventKeyboard::KeyCode key) {
	if (keysLast.find(key) != keysLast.end())
		return keysLast[key];
	return false;
}

float KeyboardManager::keyPressedDuration(cocos2d::EventKeyboard::KeyCode key) {
	if (keys.find(key) != keys.end())
		return float (clock() - keys[key]) / (float) CLOCKS_PER_SEC;
	return 0;
}
