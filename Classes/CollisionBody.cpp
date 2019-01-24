#include "CollisionBody.h"

namespace Retry
{

void Retry::CollisionBody::setDebugDraw(bool doDebugDraw)
{
	if (doDebugDraw) this->addChild(hitBox);
	else             this->removeChild(hitBox);
}

Retry::CollisionRect::CollisionRect(cocos2d::Vec2 size, cocos2d::Vec2 position) 
{
	this->size = size;
	this->setPosition(position);
	this->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	this->type = RECT;

	hitBox = cocos2d::DrawNode::create();
	hitBox->drawSolidRect(position, position + size, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.3f));
}

Retry::CollisionRect::CollisionRect(float width, float height, cocos2d::Vec2 position)
{
	this->size = cocos2d::Vec2(width, height);
	this->setPosition(position);
	this->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	this->type = RECT;

	hitBox = cocos2d::DrawNode::create();
	hitBox->drawSolidRect(position, position + size, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.3f));
}

CollisionCircle::CollisionCircle(float radius, cocos2d::Vec2 position)
{
	this->radius = radius;
	this->setPosition(position);
	this->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	this->type = RECT;

	hitBox = cocos2d::DrawNode::create();
	hitBox->drawSolidCircle(position, radius, 0, floor(6.28f * radius / segmentLength), cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.3f));
}

}
