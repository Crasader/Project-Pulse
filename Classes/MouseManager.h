#ifndef MOUSE_MANAGER
#define MOUSE_MANAGER

#include <ctime>
#include "cocos2d.h"

namespace Retry
{

enum class MouseButton;

class Mouse abstract
{
public:

	static void refresh(float delta);

	static void createListener(cocos2d::EventDispatcher* dispatcher, cocos2d::Node* node);

	static void updateButton(MouseButton button, bool isPressed);

	static bool isButtonPressed(MouseButton button);
	static bool isButtonDown(MouseButton button);
	static bool isButtonUp(MouseButton button);

	static float buttonPressedDuration(MouseButton button);

	static cocos2d::Vec2 getPos()
	{
		return position;
	}
	static float getX()
	{
		return position.x;
	}
	static float getY()
	{
		return position.y;
	}

	static cocos2d::Vec2 getDeltaPos()
	{
		return deltaPosition;
	}
	static float getDeltaX()
	{
		return deltaPosition.x;
	}
	static float getDeltaY()
	{
		return deltaPosition.y;
	}

	static bool isMoving()
	{
		return deltaPosition != cocos2d::Vec2(0, 0);
	}

private:
	static cocos2d::Vec2 position, deltaPosition;
	static char scroll;

	static std::unordered_map<MouseButton, clock_t> buttons;
	static std::unordered_map<MouseButton, bool> buttonsLast;

	static clock_t currentTime;
};

enum class MouseButton
{
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