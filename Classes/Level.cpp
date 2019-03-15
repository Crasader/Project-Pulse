#include "Level.h"

#include "Algorithms.h"

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;
using cocos2d::Sprite;
using cocos2d::SpriteFrame;

namespace Retry {

Level::Level(int levelNum) {

	levelDraw = cocos2d::Node::create();
	debugDraw = cocos2d::DrawNode::create();

	room = new Room;

	for (auto i : room->getTileData()) {
		tileData[i.first] = Vec3(i.second.x, i.second.y, i.second.z);
		collisionData[i.first] = i.second.w;
	}

	//auto tileset = Sprite::create("tileset.png");
	for (const auto &i : tileData) {
		if (collisionData[i.first] & 0x80)
			continue;
		if (!collisionData[i.first] || collisionData[i.first] & 0x80) continue;

		//auto sprite = Sprite::create("dirt.png");
		auto frame = SpriteFrame::create("tileset.png", Rect(Vec2(i.second.x, i.second.y) * tileSize, Size(tileSize, tileSize)));
		auto sprite = Sprite::createWithSpriteFrame(frame);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setScale(2);
		sprite->setPosition(convertLongToVec2(i.first) * tileSize * 2);
		sprite->getTexture()->setAliasTexParameters();
		levelDraw->addChild(sprite);
	}
}

char Level::getCollisionDataAt(const Vec2& v) {
	if (v.x < 0 || v.y < 0) return 0;
	if (collisionData.find(convertVec2ToLong(v / 2)) != collisionData.end())
		return collisionData.at(convertVec2ToLong(v / 2));
	return 0;
}

Vec3 Level::getTileDataAt(const Vec2& v) {
	if (v.x < 0 || v.y < 0) return 0;
	if (tileData.find(convertVec2ToLong(v / 2)) != tileData.end())
		return tileData.at(convertVec2ToLong(v / 2));
	return 0;
}

}