#include "Room.h"

#include "Algorithms.h"

namespace Retry
{

Retry::Room::Room()
{
	cocos2d::Image* roomData = new cocos2d::Image;
	roomData->initWithImageFile("level.png");
	//roomDraw = cocos2d::Node::create();

	int numChannels = roomData->getBitPerPixel() / 8;

	for (int i = 0; i < roomData->getWidth(); i++)
	{
		for (int j = 0; j < roomData->getHeight(); j++)
		{
			if (roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels] == 0)
			{
				//maxPositions.x = MAX(maxPositions.x, i);
				//maxPositions.y = MAX(maxPositions.y, j);
				Vec4 tile;
				//tile.x = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels];
				tile.x = 1;
				tile.y = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 1];
				tile.z = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 2];
				tile.w = roomData->getData()[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 3];

				tileData[convertVec2ToLong(Vec2(i, j))] = tile;
			}
		}
	}
}

}