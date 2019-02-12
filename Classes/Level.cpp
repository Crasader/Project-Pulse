#include "Level.h"

#include "Algorithms.h"

namespace Retry
{

Level::Level(int levelNum)
{
	levelDraw = cocos2d::Node::create();
	debugDraw = cocos2d::DrawNode::create();

	rooms.push_back(Room());

	for (auto r : rooms)
	{
		for (auto j : r.getTileData())
		{

			tileData[j.first] = Vec3(j.second.x, j.second.y, j.second.z);
			collisionData[j.first] = j.second.w;
		}
	}

	for (const auto &i : tileData)
	{
		auto sprite = cocos2d::Sprite::create("dirt.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setScale(tileSize / sprite->getBoundingBox().size.width);
		sprite->setPosition(convertLongToVec2(i.first) * tileSize);
		levelDraw->addChild(sprite);

		debugDraw->drawSolidRect(convertLongToVec2(i.first) * tileSize, (convertLongToVec2(i.first) + Vec2(1, 1)) * tileSize, cocos2d::Color4F(1, 0, 0, 0.3f));
	}

	//levelDraw->addChild(debugDraw);
}

char Level::getCollisionDataAt(const Vec2 &v)
{
	if (v.x < 0 || v.y < 0) return 0;
	if (collisionData.find(convertVec2ToLong(v)) != collisionData.end())
		return collisionData.at(convertVec2ToLong(v));
	return 0;
}

}