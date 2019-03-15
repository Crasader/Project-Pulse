#include "Menu.h"
#include "Button.h"

#include "MouseManager.h"
#include "KeyboardManager.h"
#include "ControllerManager.h"

using cocos2d::Node;
using cocos2d::Vec2;
using cocos2d::Size;

namespace Retry {

Menu::Menu(const Vec2& position, Node* parent)
	: root(Node::create()) {
	root->setPosition(position);
	parent->addChild(root);

	nextHeight = position.y;

	buttons.reserve(10);
}

Menu::~Menu() {
	root->removeFromParent();
	//for (auto i : buttons)
	//	delete i;
}

void Menu::update() {

	adjustForControls();

	if (indexPosition == -1) {
		// For every button, check if the mouse is over the button, and if the
		// mouse is being pressed. If the mouse leaves the button but is
		// still being pressed, then keep the button pressed
		for (auto& i : buttons) {
			if (i.isMouseOver()) {
				if (Mouse::isButtonDown(MouseButton::LEFT))
					i.setPressed();
				else if (Mouse::isButtonUp(MouseButton::LEFT))
					i.onPressed();
				else if (!Mouse::isButtonPressed(MouseButton::LEFT))
					i.setHovered();
				break;
			} else if (!Mouse::isButtonPressed(MouseButton::LEFT))
				i.setNormal();
		}
	} else {
		if (Controller::isButtonDown(ControllerButton::A) ||
			Keyboard::isKeyDown(KeyCode::SPACE))
			buttons[indexPosition].setPressed();
		else if (buttons[indexPosition].isPressed) {
			if (Controller::isButtonUp(ControllerButton::A) ||
				Keyboard::isKeyUp(KeyCode::SPACE))
				buttons[indexPosition].onPressed();
		} else
			buttons[indexPosition].setHovered();

		buttons[(indexPosition + buttons.size() - 1) % buttons.size()].setNormal();
		buttons[(indexPosition + 1) % buttons.size()].setNormal();
	}
}

void Menu::addButton(const std::string& sheet, const cocos2d::Size& dimension, const std::function<void(void)>& lambda) {
	buttons.emplace_back(sheet, Vec2(0, nextHeight), dimension);
	buttons.back().onPressed = lambda;

	root->addChild(buttons.back().sprite);

	nextHeight -= dimension.height + padding;
}

void Menu::addButton(const std::string& sheet, const cocos2d::Size& dimension, const std::string& text, const std::function<void(void)>& lambda) {
	buttons.emplace_back(sheet, Vec2(0, nextHeight), dimension);
	buttons.back().onPressed = lambda;
	buttons.back().setString(text);

	root->addChild(buttons.back().sprite);

	nextHeight -= dimension.height + padding;
}

void Menu::setPadding(const int padding) {
	this->padding = padding;
	nextHeight = 0;
	for (auto& i : buttons) {
		i.setPosition(Vec2(0, nextHeight));
		nextHeight += i.size.height + padding;
	}
}

void Menu::adjustForControls() {
	if (Mouse::getDeltaPos().getLengthSq() > 1) indexPosition = -1;
	else if (Keyboard::isKeyDown(KeyCode::UP_ARROW) ||
			 Keyboard::isKeyDown(KeyCode::W) ||
			 Controller::isAxisDown(ControllerButton::LEFT_STICK_UP) ||
			 Controller::isButtonDown(ControllerButton::DPAD_UP))
		if (indexPosition == -1) indexPosition = 0;
		else                     indexPosition = (indexPosition + buttons.size() - 1) % buttons.size();

	else if (Keyboard::isKeyDown(KeyCode::DOWN_ARROW) ||
			 Keyboard::isKeyDown(KeyCode::S) ||
			 Controller::isAxisDown(ControllerButton::LEFT_STICK_DOWN) ||
			 Controller::isButtonDown(ControllerButton::DPAD_DOWN))
		if (indexPosition == -1) indexPosition = 0;
		else                     indexPosition = (indexPosition + 1) % buttons.size();
}

}