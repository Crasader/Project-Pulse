#include "Actor.h"

Retry::KeyboardManager* Actor::keyIn = Retry::KeyboardManager::getInstance();
Retry::MouseManager* Actor::mouseIn = Retry::MouseManager::getInstance();
Retry::AudioManager* Actor::audio = Retry::AudioManager::getInstance();

void Actor::kill(float delay) {
	sprite->runAction(RemoveSelf::create());
}

void Actor::moveBy(Vec2 movement, float duration) {
	sprite->runAction(MoveBy::create(duration, movement)->clone());
}

void Actor::load(std::string path, Vec2 position) {
	sprite = Sprite::create(path);
	sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	sprite->setPosition(position);
}

