#include "Room.h"

namespace Retry
{

Retry::Room::Room(const int &directions, const int &templateNum)
{
	cocos2d::Image* roomData = new cocos2d::Image;
	roomData->initWithImageFile("level.png");
	//roomDraw = cocos2d::Node::create();
	debugDraw = cocos2d::Node::create();
	//debugDraw->retain();

	int numChannels = roomData->getBitPerPixel() / 8;

	for (int i = 0; i < roomData->getWidth(); i++)
	{
		for (int j = 0; j < roomData->getHeight(); j++)
		{
			if (roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels] == 0)
			{
				auto t = new Retry::Collision::Body(debugDraw);
				t->addRect(Vec2(i * tileSize.width, j * tileSize.height), tileSize);
				t->setDebugDraw(true);
				//debugDraw->addChild(t->getHitBox());
				terrain.push_back(t);
				//auto s = cocos2d::Sprite::create("dirt.png");
				//s->setScale(32.f / 512.f);
				//s->setAnchorPoint(Vec2::ZERO);
				//s->setPosition(Vec2(i * tileSize.width, j * tileSize.height));
				//roomDraw->addChild(s);
			}
		}
	}
	//roomDraw->addChild(debugDraw);
}

}