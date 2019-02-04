#include "Room.h"

namespace Retry
{

Retry::Room::Room(const int &directions, const int &templateNum)
{
	cocos2d::Image* roomData = new cocos2d::Image;
	roomData->initWithImageFile("level.png");
	anchor = cocos2d::Node::create();

	int numChannels = roomData->getBitPerPixel() / 8;

	for (int i = 0; i < roomData->getWidth(); i++)
	{
		for (int j = 0; j < roomData->getHeight(); j++)
		{
			if (roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels] == 0)
			{
				terrain.push_back(new Retry::Collision::Body);
				terrain.back()->addRect(Vec2(i * tileSize.width, j * tileSize.height), tileSize);
				auto s = cocos2d::Sprite::create("dirt.png");
				s->setScale(32.f / 512.f);
				s->setAnchorPoint(Vec2::ZERO);
				s->setPosition(Vec2(i * tileSize.width, j * tileSize.height));
				anchor->addChild(s);
			}
		}
	}
}

}