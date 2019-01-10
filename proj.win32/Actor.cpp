#include "Actor.h"

void Actor::kill(float delay) {
	sprite->runAction(RemoveSelf::create());
}

void Actor::load(std::string path, Vec2 position) {
	sprite = Sprite::create(path);
	sprite->setPosition(position);
	sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
}

