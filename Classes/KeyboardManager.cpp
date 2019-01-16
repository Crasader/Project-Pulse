#include "KeyboardManager.h"

using namespace Retry;

KeyboardManager* KeyboardManager::instance = 0;

clock_t KeyboardManager::currentTime = 0;

std::unordered_map<KeyCode, clock_t> KeyboardManager::keys;
std::unordered_map<KeyCode, bool> KeyboardManager::keysLast;

KeyboardManager* KeyboardManager::getInstance() {
	return instance == 0 ? (instance = new KeyboardManager()) : instance;
}

void KeyboardManager::refresh() {
	keysLast.clear();
	currentTime = clock();
}

void KeyboardManager::createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node) {
	using namespace cocos2d;
	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		KeyboardManager::getInstance()->updateKey((Retry::KeyCode) keyCode, true);
	};
	eventListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event) {
		KeyboardManager::getInstance()->updateKey((Retry::KeyCode) keyCode, false);
	};
	dispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);
}

void KeyboardManager::updateKey(KeyCode key, bool isPressed) {
	if (keys.find(key) == keys.end()) {
		if (isPressed)
			keys[key] = currentTime;
	} else if (!isPressed) {
		keys.erase(key);
	}
	keysLast[key] = isPressed;
}

bool KeyboardManager::isKeyPressed(KeyCode key) {
	return keys.find(key) != keys.end();
}

bool KeyboardManager::isKeyDown(KeyCode key) {
	if (keysLast.find(key) != keysLast.end()) {
		return keysLast[key];
	}
	return false;
}

bool KeyboardManager::isKeyUp(KeyCode key) {
	if (keysLast.find(key) != keysLast.end()) {
		return !keysLast[key];
	}
	return false;
}

float KeyboardManager::keyPressedDuration(KeyCode key) {
	if (keys.find(key) != keys.end())
		return float(currentTime - keys[key]) / (float) CLOCKS_PER_SEC;
	return 0;
}