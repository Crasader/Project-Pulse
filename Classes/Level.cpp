#include "Level.h"

namespace Retry
{

Level::Level(int levelNum)
{
	debugDraw = cocos2d::Node::create();
	room = new Room(1, 1);

	debugDraw->addChild(room->anchor);
	for (auto &i : room->terrain)
	{
		debugDraw->addChild(i->getHitBox());
		//i->setDebugDraw(true);
		//i->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));

	}
}

}