#include "CollisionBody.h"

#include <cmath>

namespace Retry {
namespace Collision {

Body::Body() {
	hitBox = cocos2d::DrawNode::create();
	hitBox->setVisible(false);
}
Body::Body(cocos2d::Node* node) {
	hitBox = cocos2d::DrawNode::create();
	node->addChild(hitBox);
	hitBox->setVisible(false);
}

void Body::setTestPosition(Vec2 pos) {
	cocos2d::Director::getInstance()->getRunningScene()->getChildByName("test")->setPosition(pos);
}

void Body::redraw() {
	hitBox->clear();

	auto p = (cocos2d::Sprite*) hitBox->getParent();
	bool isFlipped = p == nullptr ? false : p->isFlippedX();
	float newOriginX = isFlipped ? p->getContentSize().width : 0;

	for (auto &rect : collisionRects) {
		if (!isFlipped)
			hitBox->drawSolidRect(rect.position, rect.position + rect.size, debugDrawColor);
		else
			hitBox->drawSolidRect(Vec2(newOriginX - rect.position.x - rect.size.width, rect.position.y),
								  Vec2(newOriginX - rect.position.x, rect.position.y + rect.size.height),
								  debugDrawColor);
	}

	for (auto &circ : collisionCircles) {
		if (!isFlipped)
			hitBox->drawSolidCircle(circ.position, circ.radius, 0, 6.28f * circ.radius / 5, debugDrawColor);
		else
			hitBox->drawSolidCircle(Vec2(newOriginX - circ.position.x, circ.position.y), circ.radius, 0, 6.28f * circ.radius / 5, debugDrawColor);
	}

	for (auto &caps : collisionCapsules) {
		if (!isFlipped) {
			hitBox->drawSolidCircle(caps.sPosition, caps.radius, 0, 6.28f * caps.radius / 5, debugDrawColor);
			hitBox->drawSolidCircle(caps.ePosition, caps.radius, 0, 6.28f * caps.radius / 5, debugDrawColor);
			Vec2 line = (caps.ePosition - caps.sPosition).rotateByAngle(Vec2(0, 0), 3.1415f / 2).getNormalized() * caps.radius;
			Vec2* corners = new Vec2[4] { caps.sPosition + line, caps.sPosition - line, caps.ePosition - line, caps.ePosition + line };
			hitBox->drawSolidPoly(corners, 4, debugDrawColor);
		} else {
			Vec2 newSPos = Vec2(newOriginX - caps.sPosition.x, caps.sPosition.y);
			Vec2 newEPos = Vec2(newOriginX - caps.ePosition.x, caps.ePosition.y);
			hitBox->drawSolidCircle(newSPos, caps.radius, 0, 6.28f * caps.radius / 5, debugDrawColor);
			hitBox->drawSolidCircle(newEPos, caps.radius, 0, 6.28f * caps.radius / 5, debugDrawColor);
			Vec2 line = (newEPos - newSPos).rotateByAngle(Vec2(0, 0), 3.1415f / 2).getNormalized() * caps.radius;
			Vec2* corners = new Vec2[4] { newSPos + line, newSPos - line, newEPos - line, newEPos + line };
			hitBox->drawSolidPoly(corners, 4, debugDrawColor);
		}
	}

	for (auto &poly : collisionPolygons) {
		hitBox->drawSolidPoly(poly.verts.data(), poly.verts.size(), debugDrawColor);
	}
}

void Body::addRect(const Vec2 &position, const Size &size) {
	CollisionRect c;
	c.position = position;
	c.size = size;
	c.boundingBox = Rect(position, size);
	collisionRects.push_back(c);
	redraw();
}

void Body::addCircle(const Vec2 &position, const float &radius) {
	CollisionCircle c;
	c.position = position;
	c.radius = radius;
	c.boundingBox = Rect(position - Vec2(radius, radius), Size(2 * radius, 2 * radius));
	collisionCircles.push_back(c);
	redraw();
}

void Body::addCapsule(const Vec2 &sPosition, const Vec2 &ePosition, const float &radius) {
	CollisionCapsule c;
	c.sPosition = sPosition;
	c.ePosition = ePosition;
	c.radius = radius;

	float minx, miny, maxx, maxy;
	minx = MIN(sPosition.x, ePosition.x) - radius;
	miny = MIN(sPosition.y, ePosition.y) - radius;
	maxx = MAX(sPosition.x, ePosition.x) + radius;
	maxy = MAX(sPosition.y, ePosition.y) + radius;

	c.boundingBox = Rect(Vec2(minx, miny), Size(maxx - minx, maxy - miny));
	collisionCapsules.push_back(c);
	redraw();
}

void Body::addPolygon(const std::vector<Vec2> &verts) {
	CollisionPolygon c;
	c.verts = verts;
	collisionPolygons.push_back(c);
	redraw();
}

void Body::clear() {
	collisionRects.clear();
	collisionCircles.clear();
	collisionCapsules.clear();
	collisionPolygons.clear();
	redraw();
}

void Body::setDebugDraw(const bool& doDebugDraw) {
	hitBox->setVisible(doDebugDraw);
}

void Body::setDebugDrawColor(const cocos2d::Color4B &color) {
	if (cocos2d::Color4F(color) == debugDrawColor) return;
	debugDrawColor = cocos2d::Color4F(color);
	redraw();
}

void Body::setDebugDrawColor(const cocos2d::Color4F &color) {
	if (color == debugDrawColor) return;
	debugDrawColor = color;
	redraw();
}

bool Body::isCollidingWith(const Body* body) {
	for (auto i : collisionRects) {
		for (auto j : body->collisionRects)
			if (doCollisionTest(worldSpaceRect(hitBox, i), worldSpaceRect(body->hitBox, j))) return true;
		for (auto j : body->collisionCircles)
			if (doCollisionTest(worldSpaceRect(hitBox, i), worldSpaceCircle(body->hitBox, j))) return true;
		for (auto j : body->collisionCapsules)
			if (doCollisionTest(worldSpaceRect(hitBox, i), worldSpaceCapsule(body->hitBox, j))) return true;
	}

	for (auto i : collisionCircles) {
		for (auto j : body->collisionRects)
			if (doCollisionTest(worldSpaceCircle(hitBox, i), worldSpaceRect(body->hitBox, j))) return true;
		for (auto j : body->collisionCircles)
			if (doCollisionTest(worldSpaceCircle(hitBox, i), worldSpaceCircle(body->hitBox, j))) return true;
		for (auto j : body->collisionCapsules)
			if (doCollisionTest(worldSpaceCircle(hitBox, i), worldSpaceCapsule(body->hitBox, j))) return true;
	}

	for (auto i : collisionCapsules) {
		for (auto j : body->collisionRects)
			if (doCollisionTest(worldSpaceCapsule(hitBox, i), worldSpaceRect(body->hitBox, j))) return true;
		for (auto j : body->collisionCircles)
			if (doCollisionTest(worldSpaceCapsule(hitBox, i), worldSpaceCircle(body->hitBox, j))) return true;
		for (auto j : body->collisionCapsules)
			if (doCollisionTest(worldSpaceCapsule(hitBox, i), worldSpaceCapsule(body->hitBox, j))) return true;
	}
	return false;
}

Rect Body::getBoundingBox() {
	Rect boundingBox;
	for (const auto &i : collisionRects) {
		if (boundingBox.size.width == 0 && boundingBox.size.height == 0)
			boundingBox = i.boundingBox;
		boundingBox.merge(i.boundingBox);
	}
	for (const auto &i : collisionCircles) {
		if (boundingBox.size.width == 0 && boundingBox.size.height == 0)
			boundingBox = i.boundingBox;
		boundingBox.merge(i.boundingBox);
	}
	for (const auto &i : collisionCapsules) {
		if (boundingBox.size.width == 0 && boundingBox.size.height == 0)
			boundingBox = i.boundingBox;
		boundingBox.merge(i.boundingBox);
	}
	for (const auto &i : collisionPolygons) {
		if (boundingBox.size.width == 0 && boundingBox.size.height == 0)
			boundingBox = i.boundingBox;
		boundingBox.merge(i.boundingBox);
	}

	return boundingBox;
}

std::vector<Vec2> newRectVerts(const CollisionRect &rect) {
	std::vector<Vec2> newVerts;

	Vec2 newW = Vec2(rect.size.width, 0).rotateByAngle(Vec2(0, 0), rect.rotation);
	Vec2 newH = Vec2(0, rect.size.height).rotateByAngle(Vec2(0, 0), rect.rotation);
	Vec2 newWH = Vec2(rect.size.width, rect.size.height).rotateByAngle(Vec2(0, 0), rect.rotation);

	newVerts.push_back(rect.position);
	newVerts.push_back(rect.position + newH);
	newVerts.push_back(rect.position + newWH);
	newVerts.push_back(rect.position + newW);

	return newVerts;
}

void getInterval(const Vec2 &axis, const std::vector<Vec2> &verts, float &min, float &max) {
	min = max = axis.dot(verts[0]);
	for (int i = 1, n = verts.size(); i < n; i++) {
		float value = axis.dot(verts[i]);
		if (value < min) min = value;
		if (value > max) max = value;
	}
}

Vec2 getNormal(const Vec2 &u) {
	return Vec2(-u.y, u.x);
}

bool doSATCollision(const std::vector<Vec2> &o1, const std::vector<Vec2> &o2) {
	float min1, max1, min2, max2;

	for (int i = 0, n = o1.size(); i < n; i++) {
		getInterval(getNormal(o1[(i + 1) % n] - o1[i]), o1, min1, max1);
		getInterval(getNormal(o1[(i + 1) % n] - o1[i]), o2, min2, max2);
		if (max1 < min2 || max2 < min1) return false;
	}
	for (int i = 0, n = o2.size(); i < n; i++) {
		getInterval(getNormal(o2[(i + 1) % n] - o2[i]), o1, min1, max1);
		getInterval(getNormal(o2[(i + 1) % n] - o2[i]), o2, min2, max2);
		if (max1 < min2 || max2 < min1) return false;
	}

	return true;
}


bool Body::doCollisionTest(const CollisionRect &rect1, const CollisionRect &rect2) {
	if (rect1.rotation == 0 && rect2.rotation == 0)
		if (rect1.position.x < rect2.position.x + rect2.size.width &&
			rect1.position.x + rect1.size.width > rect2.position.x &&
			rect1.position.y < rect2.position.y + rect2.size.height &&
			rect1.position.y + rect1.size.height > rect2.position.y) {
			return true;
		} else return false;

	std::vector<Vec2> o1 = newRectVerts(rect1);
	std::vector<Vec2> o2 = newRectVerts(rect2);

	return doSATCollision(o1, o2);
}

bool doRectCircleCollision(const Vec2 &A, const Vec2 &AB, const Vec2 &AD, const Vec2 &P, const float &R) {
	if (pointInRect(P, A, AB, AD) ||
		doLineCircleCollision(A, A + AD, P, R) ||
		doLineCircleCollision(A + AB, A + AB + AD, P, R) ||
		doLineCircleCollision(A + AB + AD, A + AD, P, R) ||
		doLineCircleCollision(A + AD, A, P, R)) {
		return true;
	}
	return false;
}

bool Body::doCollisionTest(const CollisionRect &rect, const CollisionCircle &circ) {
	Vec2 newW = Vec2(rect.size.width, 0).rotateByAngle(Vec2(0, 0), rect.rotation);
	Vec2 newH = Vec2(0, rect.size.height).rotateByAngle(Vec2(0, 0), rect.rotation);
	return doRectCircleCollision(rect.position, newW, newH, circ.position, circ.radius);
}

bool Body::doCollisionTest(const CollisionRect &rect, const CollisionCapsule &caps) {
	std::vector<Vec2> o1 = newRectVerts(rect);
	std::vector<Vec2> o2;
	o2.push_back(caps.sPosition - caps.toRadius);
	o2.push_back(caps.sPosition + caps.toRadius);
	o2.push_back(caps.ePosition + caps.toRadius);
	o2.push_back(caps.ePosition - caps.toRadius);
	Vec2 newW = Vec2(rect.size.width, 0).rotateByAngle(Vec2(0, 0), rect.rotation);
	Vec2 newH = Vec2(0, rect.size.height).rotateByAngle(Vec2(0, 0), rect.rotation);

	if (doRectCircleCollision(rect.position, newW, newH, caps.sPosition, caps.radius) ||
		doRectCircleCollision(rect.position, newW, newH, caps.ePosition, caps.radius) ||
		doSATCollision(o1, o2)) {
		return true;
	}
	return false;
}

bool Body::doCollisionTest(const CollisionCircle &circ1, const CollisionCircle &circ2) {
	if ((circ1.position - circ2.position).getLengthSq() <
		(circ1.radius + circ2.radius) * (circ1.radius + circ2.radius)) {
		return true;
	}
	return false;
}

bool Body::doCollisionTest(const CollisionCircle &circ, const CollisionCapsule &caps) {
	float rSq = (circ.radius + caps.radius) * (circ.radius + caps.radius);
	if ((caps.sPosition - circ.position).getLengthSq() < rSq ||
		(caps.ePosition - circ.position).getLengthSq() < rSq ||
		doRectCircleCollision(caps.sPosition - caps.toRadius,
		2 * caps.toRadius,
		caps.ePosition - caps.sPosition,
		circ.position,
		circ.radius)) {
		return true;
	}
	return false;
}

bool Body::doCollisionTest(const CollisionCapsule &caps1, const CollisionCapsule &caps2) {
	std::vector<Vec2> o1;
	o1.push_back(caps1.sPosition - caps1.toRadius);
	o1.push_back(caps1.sPosition + caps1.toRadius);
	o1.push_back(caps1.ePosition + caps1.toRadius);
	o1.push_back(caps1.ePosition - caps1.toRadius);
	std::vector<Vec2> o2;
	o2.push_back(caps2.sPosition - caps2.toRadius);
	o2.push_back(caps2.sPosition + caps2.toRadius);
	o2.push_back(caps2.ePosition + caps2.toRadius);
	o2.push_back(caps2.ePosition - caps2.toRadius);

	float rSq = (caps1.radius + caps2.radius) * (caps1.radius + caps2.radius);
	if ((caps1.sPosition - caps2.sPosition).getLengthSq() < rSq ||
		(caps1.sPosition - caps2.ePosition).getLengthSq() < rSq ||
		(caps1.ePosition - caps2.sPosition).getLengthSq() < rSq ||
		(caps1.ePosition - caps2.ePosition).getLengthSq() < rSq ||
		doRectCircleCollision(caps1.sPosition - caps1.toRadius, 2 * caps1.toRadius, caps1.ePosition - caps1.sPosition, caps2.sPosition, caps2.radius) ||
		doRectCircleCollision(caps1.sPosition - caps1.toRadius, 2 * caps1.toRadius, caps1.ePosition - caps1.sPosition, caps2.ePosition, caps2.radius) ||
		doRectCircleCollision(caps2.sPosition - caps2.toRadius, 2 * caps2.toRadius, caps2.ePosition - caps2.sPosition, caps1.sPosition, caps1.radius) ||
		doRectCircleCollision(caps2.sPosition - caps2.toRadius, 2 * caps2.toRadius, caps2.ePosition - caps2.sPosition, caps1.ePosition, caps1.radius) ||
		doSATCollision(o1, o2)) {
		return true;
	}

	return false;
}

CollisionRect worldSpaceRect(const cocos2d::Node* p, const CollisionRect &c) {
	// TODO: Make the x position relative to right side when it's flipped
	auto parentSprite = (cocos2d::Sprite*) p->getParent();
	CollisionRect newShape;
	newShape.position = c.position;
	if (parentSprite->isFlippedX()) {
		newShape.position.x = parentSprite->getContentSize().width - c.position.x - c.size.width;
	}
	newShape.position = p->convertToWorldSpace(newShape.position) / cocos2d::Director::getInstance()->getRunningScene()->getScale();

	float totalScale = 1;
	for (cocos2d::Node* n = (cocos2d::Node*) p; n != nullptr; n = n->getParent()) {
		totalScale *= n->getScale();
	}

	newShape.size = c.size * totalScale;
	newShape.rotation = -p->getParent()->getRotation() * 3.1415 / 180;
	return newShape;
}

CollisionCircle worldSpaceCircle(const cocos2d::Node* p, const CollisionCircle &c) {
	auto parentSprite = (cocos2d::Sprite*) p->getParent();
	CollisionCircle newShape;
	newShape.position = c.position;
	if (parentSprite->isFlippedX())
		newShape.position.x = parentSprite->getContentSize().width - c.position.x;
	newShape.position = p->convertToWorldSpace(newShape.position) / cocos2d::Director::getInstance()->getRunningScene()->getScale();
	newShape.radius = c.radius * p->getParent()->getScale();
	return newShape;
}

CollisionCapsule worldSpaceCapsule(const cocos2d::Node* p, const CollisionCapsule &c) {
	auto parentSprite = (cocos2d::Sprite*) p->getParent();
	CollisionCapsule newShape;
	newShape.sPosition = c.sPosition;
	newShape.ePosition = c.ePosition;
	if (parentSprite->isFlippedX()) {
		newShape.sPosition.x = parentSprite->getContentSize().width - c.sPosition.x;
		newShape.ePosition.x = parentSprite->getContentSize().width - c.ePosition.x;
	}
	newShape.sPosition = p->convertToWorldSpace(newShape.sPosition) / cocos2d::Director::getInstance()->getRunningScene()->getScale();
	newShape.ePosition = p->convertToWorldSpace(newShape.ePosition) / cocos2d::Director::getInstance()->getRunningScene()->getScale();
	newShape.radius = c.radius * p->getParent()->getScale();
	newShape.toRadius = (newShape.ePosition - newShape.sPosition).rotateByAngle(Vec2(0, 0), 3.1415f / 2).getNormalized() * newShape.radius;
	newShape.rotation = -p->getParent()->getRotation() * 3.1415 / 180;
	return newShape;
}

CollisionPolygon worldSpacePolygon(const cocos2d::Node* p, const CollisionPolygon &c) {
	CollisionPolygon newShape;
	newShape.verts = c.verts;
	for (auto &i : newShape.verts)
		i = p->convertToWorldSpace(i) / cocos2d::Director::getInstance()->getRunningScene()->getScale();
	return newShape;
}

Vec2 project(const Vec2 &u, const Vec2 &v) {
	return v * u.dot(v) / v.lengthSquared();
}

bool pointInRect(const Vec2 &pos, const Vec2 &A, const Vec2 &AB, const Vec2 &AD) {
	float APAB = (pos - A).dot(AB);
	float APAD = (pos - A).dot(AD);
	if (0 <= APAB && APAB <= AB.getLengthSq() && 0 <= APAD && APAD <= AD.getLengthSq()) {
		return true;
	}
	return false;
}

bool doLineCircleCollision(const Vec2 &A, const Vec2 &B, const Vec2 &P, const float &R) {
	Vec2 side = B - A;
	Vec2 proj = A + project(P - A, side);
	float rSq = R * R;
	if (((P - A).getLengthSq() < rSq ||
		(P - B).getLengthSq() < rSq) ||
		((proj - P).getLengthSq() < rSq &&
		(proj - A).getLengthSq() < side.getLengthSq() &&
		(proj - B).getLengthSq() < side.getLengthSq())) {
		return true;
	}
	return false;
}

}

}
