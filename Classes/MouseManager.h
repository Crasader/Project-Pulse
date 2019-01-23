#ifndef MOUSE_MANAGER
#define MOUSE_MANAGER

#include <ctime>
#include "cocos2d.h"

namespace Retry {

enum class MouseButton;

class MouseManager {
private:
	static MouseManager* instance;
	MouseManager() = default;
public:
	static MouseManager* getInstance();

	void refresh();

	void createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node);

	void updateButton(MouseButton button, bool isPressed);

	bool isButtonPressed(MouseButton button);
	bool isButtonDown(MouseButton button);
	bool isButtonUp(MouseButton button);

	float buttonPressedDuration(MouseButton button);

	cocos2d::Vec2 getPos() { return position; }
	float getX() { return position.x; }
	float getY() { return position.y; }

	cocos2d::Vec2 getDeltaPos() { return deltaPosition; }
	float getDeltaX() { return deltaPosition.x; }
	float getDeltaY() { return deltaPosition.y; }

	bool isMoving() { return deltaPosition != cocos2d::Vec2(0, 0); }

private:
	static cocos2d::Vec2 position, deltaPosition;
	static char scroll;

	static std::unordered_map<MouseButton, clock_t> buttons;
	static std::unordered_map<MouseButton, bool> buttonsLast;

	static clock_t currentTime;
};

enum class MouseButton {
	UNSET = -1,
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2,
	BUTTON_4 = 3,
	BUTTON_5 = 4,
	BUTTON_6 = 5,
	BUTTON_7 = 6,
	BUTTON_8 = 7
};
}

#endif