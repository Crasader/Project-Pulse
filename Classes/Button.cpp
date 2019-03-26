#include "Button.h"

#include "ControllerManager.h"
#include "KeyboardManager.h"
#include "MouseManager.h"

#include "cocos2d.h"

using cocos2d::Sprite;
using cocos2d::SpriteFrame;
using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

namespace Retry {

Button::Button(const std::string& sheet, const Vec2& position, const Size& dimensions)
	: position(position), size(dimensions) {
	auto temp = Sprite::create(sheet);

	normal = Sprite::createWithTexture(temp->getTexture(), Rect(Vec2::ZERO, dimensions));
	hovered = Sprite::createWithTexture(temp->getTexture(), Rect(Vec2(0, dimensions.height), dimensions));
	pressed = Sprite::createWithTexture(temp->getTexture(), Rect(Vec2(0, dimensions.height * 2), dimensions));

	sprite = Sprite::createWithSpriteFrame(normal->getSpriteFrame());
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(position);
	sprite->retain();

	sprite->addChild(normal);
	normal->setVisible(false);
	sprite->addChild(hovered);
	hovered->setVisible(false);
	sprite->addChild(pressed);
	pressed->setVisible(false);

	label = cocos2d::Label::create();
	label->setPosition(sprite->getAnchorPointInPoints());
	label->setAlignment(cocos2d::TextHAlignment::CENTER);
	label->setSystemFontSize(30);
	sprite->addChild(label);

	sprite->getTexture()->setAliasTexParameters();
}
Button::~Button() {
	//sprite->removeFromParent();

	//normal->autorelease();
	//hovered->autorelease();
	//pressed->autorelease();
}

void Button::setNormal() {
	sprite->setSpriteFrame(normal->getSpriteFrame());
	isHovered = isPressed = false;
}

void Button::setHovered() {
	sprite->setSpriteFrame(hovered->getSpriteFrame());
	isHovered = !(isPressed = false);
}

void Button::setPressed() {
	sprite->setSpriteFrame(pressed->getSpriteFrame());
	isPressed = !(isHovered = false);
}

void Button::setPosition(const cocos2d::Vec2& position) {
	this->position = position;
	this->sprite->setPosition(position);
}

void Button::setScale(const float scale) {
	this->scale = scale;
	this->sprite->setScale(scale);
}

bool Button::isMouseOver() const {
	Vec2 worldPosition = sprite->convertToWorldSpace(Vec2::ZERO);
	float scale = [&]()->float {
		float s = 1;
		for (cocos2d::Node* n = sprite; n != nullptr; n = n->getParent()) s *= n->getScale();
		return s;
	}();
	return (Mouse::getX() > worldPosition.x &&
			Mouse::getY() > worldPosition.y &&
			Mouse::getX() < worldPosition.x + size.width * scale &&
			Mouse::getY() < worldPosition.y + size.height * scale);
}

void Button::setString(const std::string& string) {
	label->setString(string);
}

}