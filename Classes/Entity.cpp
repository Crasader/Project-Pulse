#include "Entity.h"

#include "cocos2d.h"

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

Entity::Entity(std::string path, cocos2d::Vec2 position)
{
	init(path, position);
}

Entity::~Entity() {
	sprite->removeFromParent();
}

void Entity::init(std::string path, cocos2d::Vec2 position)
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

	new (&hurtBox) Retry::Collision::Body(sprite);
	new (&hitBox)  Retry::Collision::Body(sprite);
	hurtBox.setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
	hitBox.setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
}

void Entity::kill(float delay)
{
	sprite->runAction(cocos2d::RemoveSelf::create());
}

void Entity::moveBy(cocos2d::Vec2 movement)
{
	setPosition(position + movement);
	//position += movement;
	//sprite->setPosition(position);
}

void Entity::initAnimation(std::string action, std::string file, Vec2 startCell, Vec2 frameSize, int numFrames)
{
	//TODO: Add more functionality
	auto temp = cocos2d::Animation::create();
	int width = sprite->getTexture()->getPixelsWide() / frameSize.x;
	for (int i = 0; i < numFrames; i++)
	{
		Vec2 startPosition((((int) startCell.x + i) % width) * frameSize.x, (startCell.y + ((int) startCell.x + i) / width) * frameSize.y);
		temp->addSpriteFrame(cocos2d::SpriteFrame::create(file.c_str(), Rect(startPosition, Size(frameSize))));
	}
	temp->setLoops(1);

	animations[action] = temp;
	animations[action]->retain();
}

void Entity::runAnimation(std::string action, float totalTime)
{
	if (animations.find(action) == animations.end()) return;

	auto currentAnimate = (cocos2d::Animate*)sprite->getActionByTag('anim');

	if (totalTime > 0) animations[action]->setDelayPerUnit(totalTime);

	if (currentAnimation != action)
	{
		currentAnimation = action;
		sprite->stopAllActionsByTag('anim');
		auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
		tempAnim->setTag('anim');
		sprite->runAction(tempAnim);
	} else if (currentAnimate == nullptr)
	{
		sprite->stopAllActionsByTag('anim');
		auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
		tempAnim->setTag('anim');
		sprite->runAction(tempAnim);
	} else
	{
		float elapsed = currentAnimate->getElapsed();
		if (this->totalTime != totalTime && elapsed > 0)
		{
			sprite->stopAllActionsByTag('anim');
			auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
			tempAnim->setTag('anim');
			sprite->runAction(tempAnim);
			((cocos2d::Animate*)sprite->getActionByTag('anim'))->step(elapsed);
		}
	}
	this->totalTime = totalTime;
	//cocos2d::log("%f", ((cocos2d::Animate*)sprite->getActionByTag('anim')));
}

}