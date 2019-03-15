#include "Room.h"

#include "Algorithms.h"

namespace Retry {

Retry::Room::Room() {
	cocos2d::Image* roomData = new cocos2d::Image;
	roomData->initWithImageFile("level.png");
	//roomDraw = cocos2d::Node::create();

	int numChannels = roomData->hasAlpha() ? 4 : 3;

	unsigned char* data = roomData->getData();

	for (int i = 0; i < roomData->getWidth(); i++) {
		for (int j = 0; j < roomData->getHeight(); j++) {
				// x = sprite sheet x
				// y = sprite sheet y
				// z = sprite sheet id
				// w = collision info
			Vec4 tile;
			tile.x = data[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels];
			tile.y = data[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 1];
			tile.z = data[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 2];
			int coll = data[(i + (roomData->getHeight() - j - 1) * roomData->getWidth()) * numChannels + 3];
			tile.w = !coll ? 0 : 1 << (255 - coll);
			//tile.w = 0x01;

			tileData[convertVec2ToLong(Vec2(i, j))] = tile;
		}
	}
}

}