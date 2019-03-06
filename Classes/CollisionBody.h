#ifndef COLLISION_BODY
#define COLLISION_BODY

#include "cocos2d.h"

namespace Retry
{
namespace Collision
{
using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

struct CollisionRect;
struct CollisionCircle;
struct CollisionCapsule;
struct CollisionPolygon;

class Body
{
public:
	Body();
	Body(cocos2d::Node* node);

	cocos2d::DrawNode* getHitBox() const { return hitBox; }

	void setParent(cocos2d::Node* node) { node->addChild(hitBox); }

	void redraw();

	void addRect(const Vec2 &position, const Size &size);
	void addCircle(const Vec2 &position, const float &radius);
	void addCapsule(const Vec2 &sPosition, const Vec2 &ePosition, const float &radius);
	void addPolygon(const std::vector<Vec2> &verts);

	std::vector<CollisionRect> getRects() { return collisionRects; }
	std::vector<CollisionCircle> getCircles() { return collisionCircles; }
	std::vector<CollisionCapsule> getCapsules() { return collisionCapsules; }
	std::vector<CollisionPolygon> getPolygons() { return collisionPolygons; }

	void clear();

	void setDebugDraw(const bool &doDebugDraw);
	void setDebugDrawColor(const cocos2d::Color4F &color);
	void setDebugDrawColor(const cocos2d::Color4B &color);

	bool isCollidingWith(const Body* body);

	Rect getBoundingBox();

	static void setTestPosition(Vec2 pos);
private:
	cocos2d::DrawNode* hitBox;

	cocos2d::Color4F debugDrawColor = cocos2d::Color4F(1.0f, 0.0f, 0.0f, 0.3f);

	std::vector<CollisionRect> collisionRects;
	std::vector<CollisionCircle> collisionCircles;
	std::vector<CollisionCapsule> collisionCapsules;
	std::vector<CollisionPolygon> collisionPolygons;

	bool doCollisionTest(const CollisionRect &rect1, const CollisionRect &rect2);
	bool doCollisionTest(const CollisionRect &rect, const CollisionCircle &circ);
	bool doCollisionTest(const CollisionCircle &circ, const CollisionRect &rect) { return doCollisionTest(rect, circ); }
	bool doCollisionTest(const CollisionRect &rect, const CollisionCapsule &caps);
	bool doCollisionTest(const CollisionCapsule &caps, const CollisionRect &rect) { return doCollisionTest(rect, caps); }

	bool doCollisionTest(const CollisionCircle &circ1, const CollisionCircle &circ2);
	bool doCollisionTest(const CollisionCircle &circ, const CollisionCapsule &caps);
	bool doCollisionTest(const CollisionCapsule &caps, const CollisionCircle &circ) { return doCollisionTest(circ, caps); };

	bool doCollisionTest(const CollisionCapsule &caps1, const CollisionCapsule &caps2);
};

CollisionRect worldSpaceRect(const cocos2d::Node* p, const CollisionRect &c);
CollisionCircle worldSpaceCircle(const cocos2d::Node* p, const CollisionCircle &c);
CollisionCapsule worldSpaceCapsule(const cocos2d::Node* p, const CollisionCapsule &c);
CollisionPolygon worldSpacePolygon(const cocos2d::Node* p, const CollisionPolygon &c);

Vec2 project(const Vec2 &u, const Vec2 &v);

bool pointInRect(const Vec2 &pos, const Vec2 &A, const Vec2 &AB, const Vec2 &AD);

bool doLineCircleCollision(const Vec2 &B, const Vec2 &A, const Vec2 &P, const float &R);

struct CollisionRect
{
	bool doDraw = true;

	Vec2 position;
	Size size;

	float rotation = 0;

	Rect boundingBox;
};

struct CollisionCircle
{
	bool doDraw = true;

	Vec2 position;
	float radius;

	Rect boundingBox;
};

struct CollisionCapsule
{
	bool doDraw = true;

	Vec2 sPosition;
	Vec2 ePosition;
	float radius;

	float rotation = 0;
	Vec2 toRadius;

	Rect boundingBox;
};

struct CollisionPolygon
{
	bool doDraw = true;

	std::vector<Vec2> verts;

	Rect boundingBox;
};

}

}

#endif