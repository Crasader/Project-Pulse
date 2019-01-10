#pragma once
#include "Actor.h"
class Player : Actor {
public:
	Player(std::string path, Vec2 pos);

	void update(float dt);
};

