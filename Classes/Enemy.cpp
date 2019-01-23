#include "Enemy.h"

namespace Retry
{

Enemy::Enemy(std::string path, cocos2d::Vec2 pos)
{
	load(path, pos);
}

void Enemy::update(float dt)
{

}

}