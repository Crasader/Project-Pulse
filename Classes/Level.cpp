#include "Level.h"

namespace Retry
{

Level::Level(int levelNum)
{
	levelDraw = cocos2d::Node::create();

	rooms.push_back(Room(1, 1));

	for (int i = 0; i < rooms.size(); i++)
	{
		levelDraw->addChild(rooms[i].getDebugDraw());
	}
}

}