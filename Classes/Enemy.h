#pragma once

#include "Actor.h"

namespace Retry {

class Player;

class Enemy : public Actor {
public:
	Enemy(const std::string& path, const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO);

	virtual void update(const float delta) override;

	virtual void updateAI(Player* player, const float delta) = 0;
};

class GoonEnemy : public Enemy {
public:
	GoonEnemy(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO);

	virtual void update(const float delta) override;

	virtual void updateAI(Player* player, const float delta) override;
};



class TutorialEnemy : public GoonEnemy {
public:
	TutorialEnemy(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO);

	virtual void update(const float delta) override;

	virtual void updateAI(Player* player, const float delta) override;
};

}

