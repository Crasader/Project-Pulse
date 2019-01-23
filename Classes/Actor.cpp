#include "Actor.h"

namespace Retry
{

void Actor::load(std::string path, cocos2d::Vec2 position)
{
	sprite = cocos2d::Sprite::create(path);
	if (sprite == nullptr)
	{
		cocos2d::log("Sprite %s could not be loaded!", path.c_str());
		auto rt = cocos2d::RenderTexture::create(128, 128);
		rt->beginWithClear(1, 0, 1, 1);
		rt->end();
		sprite = cocos2d::Sprite::createWithTexture(rt->getSprite()->getTexture());
	}
	sprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	sprite->setPosition(position);
}

void Actor::kill(float delay)
{
	sprite->runAction(cocos2d::RemoveSelf::create());
}

void Actor::moveBy(cocos2d::Vec2 movement, float duration)
{
	sprite->runAction(cocos2d::MoveBy::create(duration, movement)->clone());
}

void Actor::bufferAction(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		actionBuffer[action];
}

void Actor::updateActionBuffer()
{

}

bool Actor::isActionPressed(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].pressed;
	return false;
}

bool Actor::isActionDown(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].down;
	return false;
}

bool Actor::isActionUp(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].up;
	return false;
}

float Actor::actionPressedDuration(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].time;
	return false;
}

}