#ifndef COLLISION_BODY
#define COLLISION_BODY

#include "cocos2d.h"
#include "2d/CCDrawNode.h"

namespace Retry
{

enum CollideType
{
	RECT,
	CIRCLE,
	CAPSULE
};

class CollisionBody : public cocos2d::Node
{
public:
	CollisionBody() { hitBox = cocos2d::DrawNode::create(); };

	void setDebugDraw(bool doDebugDraw);
	void setColor(cocos2d::Color4F color);
	void setColor(unsigned char r, unsigned char g, unsigned char b);

protected:
	cocos2d::DrawNode* hitBox;

	CollideType type;
};

class CollisionRect : CollisionBody
{
public:
	CollisionRect() = default;
	CollisionRect(cocos2d::Vec2 size, cocos2d::Vec2 position = cocos2d::Vec2(0, 0));
	CollisionRect(float width, float height, cocos2d::Vec2 position = cocos2d::Vec2(0, 0));
private:
	cocos2d::Vec2 size;
};

class CollisionCircle : CollisionBody
{
public:
	CollisionCircle() = default;
	CollisionCircle(float radius, cocos2d::Vec2 position = cocos2d::Vec2(0, 0));
private:
	float radius;
	
	const float segmentLength = 20;
};

class CollisionCapsule : CollisionBody
{
public:
	CollisionCapsule() = default;
	CollisionCapsule(float radius, float length, float angle = 0, cocos2d::Vec2 position = cocos2d::Vec2(0, 0));
private:

};




}

#endif