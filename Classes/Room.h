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

	std::unordered_map<long, Vec4> getTileData() { return tileData; };

private:
	std::unordered_map<long, Vec4> tileData;

	const cocos2d::Size tileSize = cocos2d::Size(32, 32);

	Vec2 worldPosition;
};

}

#endif