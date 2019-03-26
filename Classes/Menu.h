#pragma once
#include <vector>
#include <string>
#include "cocos2d.h"
#include "Button.h"

namespace Retry {

class Menu {
public:
	Menu(const cocos2d::Vec2& position, cocos2d::Node* parent);
	~Menu();

	void update();

	void addButton(const std::string& sheet, const cocos2d::Size& dimension, const std::function<void(void)>& lambda);
	void addButton(const std::string& sheet, const cocos2d::Size& dimension, const std::string& text, const std::function<void(void)>& lambda);

	void setPadding(const int padding);

	void setPosition(const cocos2d::Vec2 position) { root->setPosition(position); }

	void setScale(const int scale) { root->setScale(scale); }

protected:
	void adjustForControls();

	cocos2d::Node* root;

	std::vector<Button> buttons;
	int indexPosition = -1;

	int nextHeight = 0;

	int padding = 30;
};

}