#pragma once
#include <string>
#include "cocos2d.h"


namespace Retry {

class Menu;

class Button {
public:
	Button(const std::string& sheet, const cocos2d::Vec2& position, const cocos2d::Size& dimensions);
	virtual ~Button();

	void setNormal();
	bool getIsNormal() const { return !isHovered && !isPressed; }

	void setHovered();
	bool getIsHovered() const { return isHovered; }

	void setPressed();
	bool getIsPressed() const { return isPressed; }

	cocos2d::Vec2 getPosition() const { return position; }
	cocos2d::Size getSize() const { return size * scale; }
	float getScale() const { return scale; }

	void setPosition(const cocos2d::Vec2& position);
	void setScale(const float scale);

	bool isMouseOver() const;

	void setString(const std::string& string);

protected:
	bool isHovered;
	bool isPressed;

	cocos2d::Vec2 position;
	cocos2d::Size size;
	float scale;

	cocos2d::Sprite* normal;
	cocos2d::Sprite* hovered;
	cocos2d::Sprite* pressed;

	cocos2d::Label* label;

	cocos2d::Sprite* sprite;

	std::function<void(void)> onPressed;

	friend class Menu;
};

}