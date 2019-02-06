#ifndef RETRY_ROOM
#define RETRY_ROOM

#include "cocos2d.h"
#include "CollisionBody.h"

#include <vector>

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Vec3;

class Room
{
public:
	Room() = default;
	Room(const int &directions, const int &templateNum);

	cocos2d::Node* getDebugDraw() { return debugDraw; }

	cocos2d::Node* getRoomDraw() { return roomDraw; }

	std::vector<Retry::Collision::Body*> getTerrain() { return terrain; }

private:
	cocos2d::Node* roomDraw = nullptr;
	cocos2d::Node* debugDraw = nullptr;

	std::vector<Retry::Collision::Body*> terrain;

	const cocos2d::Size tileSize = cocos2d::Size(32, 32);

	Vec2 worldPosition = Vec2(0, 0);

	std::vector<Vec3> tileData;
};

}

#endif