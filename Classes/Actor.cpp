#include "Actor.h"

void Actor::load(std::string path, Vec2 position) {
	sprite = Sprite::create(path);
	if (sprite == nullptr) {
		cocos2d::log("Sprite %s could not be loaded!", path.c_str());
		auto rt = cocos2d::RenderTexture::create(128, 128);
		rt->beginWithClear(1, 0, 1, 1);
		rt->end();
		sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
	}
	sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	sprite->setPosition(position);
}

void Actor::kill(float delay) {
	sprite->runAction(RemoveSelf::create());
}

void Actor::moveBy(Vec2 movement, float duration) {
	sprite->runAction(MoveBy::create(duration, movement)->clone());
}

void Actor::bufferAction(std::string action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		actionBuffer[action];
}

void Actor::updateActionBuffer() {

}

bool Actor::isActionPressed(std::string action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].pressed;
	return false;
}

bool Actor::isActionDown(std::string action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].down;
	return false;
}

bool Actor::isActionUp(std::string action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].up;
	return false;
}

float Actor::actionPressedDuration(std::string action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].time;
	return false;
}





