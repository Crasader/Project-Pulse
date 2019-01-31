#include "Entity.h"

#include "cocos2d.h"

namespace Retry
{

Entity::Entity(std::string path, cocos2d::Vec2 position)
{
	init(path, position);
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
	position += movement;
	sprite->setPosition(position);
}

void Entity::initAnimation(std::string action, std::string file, cocos2d::Vec2 startCell, cocos2d::Vec2 frameSize, int numFrames)
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

void Entity::runAnimation(std::string action, float totalTime)
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

}