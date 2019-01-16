#pragma once
#include "Actor.h"
class Enemy : public Actor {
public:
	Enemy(std::string path, Vec2 pos);

	void update(float dt);
};

