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
	this->position = position;
}

void Actor::kill(float delay)
{
	sprite->runAction(cocos2d::RemoveSelf::create());
}

void Actor::moveBy(cocos2d::Vec2 movement)
{
	position += movement;
	sprite->setPosition(position);
}

void Actor::bufferAction(std::string action)
{
	if (actionBuffer.find(action) != actionBuffer.end())
		actionBuffer[action];
}

void Actor::initAnimation(std::string action, std::string file, cocos2d::Vec2 startCell, cocos2d::Vec2 frameSize, int numFrames)
{
	//TODO: Add more functionality
	auto temp = cocos2d::Animation::create();
	for (int i = 0; i < numFrames; i++)
	{
		cocos2d::Vec2 startPosition((startCell.x + i) * frameSize.x, (startCell.y) * frameSize.y);
		temp->addSpriteFrame(cocos2d::SpriteFrame::create(file.c_str(), cocos2d::Rect(startPosition, cocos2d::Size(frameSize))));
	}
	temp->setLoops(-1);
	
	animations[action] = temp;
	animations[action]->retain();
}

void Actor::runAnimation(std::string action, float totalTime)
{
	this->totalTime = totalTime;
	if (totalTime > 0) animations[action]->setDelayPerUnit(totalTime);
	if (currentAction != action)
	{
		currentAction = action;
		sprite->stopActionByTag('anim');
		auto tempAnim = cocos2d::Animate::create(animations[action]->clone())->clone();
		tempAnim->setTag('anim');
		sprite->runAction(tempAnim);
	}
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