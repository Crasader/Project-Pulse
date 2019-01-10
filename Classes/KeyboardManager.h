#ifndef KEYBOARD_MANAGER
#define KEYBOARD_MANAGER

#include <ctime>
#include "cocos2d.h"

class KeyboardManager {
private:
	static KeyboardManager* instance;
	KeyboardManager() = default;
public:
	static KeyboardManager* getInstance();

	cocos2d::EventListenerKeyboard* createListener();

	void updateKey(cocos2d::EventKeyboard::KeyCode key, bool isPressed);
	void flush() { keysLast.clear(); };

	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode key);
	bool isKeyDown(cocos2d::EventKeyboard::KeyCode key);
	bool isKeyUp(cocos2d::EventKeyboard::KeyCode key);

	float keyPressedDuration(cocos2d::EventKeyboard::KeyCode key);

private:
	static std::map<cocos2d::EventKeyboard::KeyCode, clock_t> keys;
	static std::map<cocos2d::EventKeyboard::KeyCode, bool> keysLast;

};

#endif