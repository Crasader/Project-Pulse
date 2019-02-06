#ifndef RETRY_ROOM
#define RETRY_ROOM

#include "cocos2d.h"

#include <vector>
#include <unordered_map>

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Vec3;
using cocos2d::Vec4;

class Room
{
public:
	Room();

	//cocos2d::Node* getRoomDraw() { return roomDraw; }

	//std::vector<Retry::Collision::Body*> getTerrain() { return terrain; }

	//std::vector<Vec3> getTileData() { return tileData; }

	std::unordered_map<long, Vec4> getTileData() { return tileData; };

	//Vec2 getMaxPosition() { return maxPositions; }

private:
	//cocos2d::Node* roomDraw = nullptr;

	//std::vector<Retry::Collision::Body*> terrain;

	//std::vector<Vec3> tileData;
	std::unordered_map<long, Vec4> tileData;

	const cocos2d::Size tileSize = cocos2d::Size(32, 32);

	Vec2 worldPosition;

	//Vec2 maxPositions;

};

}

#endif