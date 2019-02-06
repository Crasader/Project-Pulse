#ifndef RETRY_LEVEL
#define RETRY_LEVEL

#include <vector>

#include "Room.h"

namespace Retry
{

using cocos2d::Vec2;
using cocos2d::Vec3;

class Level
{
public:
	Level() = default;

	Level(int levelNum);

	cocos2d::Node* getLevelDraw() { return levelDraw; }

	cocos2d::Node* getDebugDraw() { return debugDraw; }

	std::vector<Room> getRooms() { return rooms; }
	
private:
	cocos2d::Node* levelDraw;
	cocos2d::Node* debugDraw;

	std::vector<Room> rooms;

};

}

#endif