#ifndef RETRY_LEVEL
#define RETRY_LEVEL

#include <vector>

#include "Room.h"

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Vec3;
using cocos2d::Vec4;

class Level
{
public:
	Level() = default;

	Level(int levelNum);

	cocos2d::Node* getLevelDraw() { return levelDraw; }

	cocos2d::Node* getDebugDraw() { return debugDraw; }

	std::vector<Room> getRooms() { return rooms; }

	//std::unordered_map<Vec2, Vec3> &getTileData() { return tileData; }
	//std::unordered_map<Vec2, char> &getCollisionData() { return collisionData; }

	char getCollisionDataAt(const Vec2 &v);

	int getTileSize() const { return tileSize; }

	//Vec2 getMaxPositions() { return maxPositions; }
	
private:
	cocos2d::Node* levelDraw;
	cocos2d::DrawNode* debugDraw;

	std::vector<Room> rooms;
	//, decltype(&vec2_hash)
	std::unordered_map<long, Vec3> tileData;
	std::unordered_map<long, char> collisionData;

	Vec2 maxPositions;

	const int tileSize = 32 * 3;

};

}

#endif