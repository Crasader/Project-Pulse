#pragma once
#include "Actor.h"

namespace Retry
{

class Enemy : public Actor
{
public:
	Enemy(std::string path, cocos2d::Vec2 pos);

	void update(float dt);
};

}

