#pragma once
#include "Actor.h"
class Enemy : Actor {
public:
	Enemy(std::string path, Vec2 pos);

	void update(float dt);
};

