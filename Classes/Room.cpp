#include "Room.h"

#include "Algorithms.h"

namespace Retry
{

Retry::Room::Room()
{
	cocos2d::Image* roomData = new cocos2d::Image;
	roomData->initWithImageFile("level2.png");
	//roomDraw = cocos2d::Node::create();

	int numChannels = roomData->getBitPerPixel() / 8;

	for (int i = 0; i < roomData->getWidth(); i++)
	{
		for (int j = 0; j < roomData->getHeight(); j++)
		{
			if (roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels] == 0)
			{
				// x = sprite sheet x
				// y = sprite sheet y
				// z = sprite sheet id
				// w = collision info
				Vec4 tile;
				tile.x = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels];
				tile.y = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 1];
				tile.z = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 2];
				//tile.w = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 3];
				tile.w = 0x02;

				tileData[convertVec2ToLong(Vec2(i, j))] = tile;
			}
		}
	}
}

}