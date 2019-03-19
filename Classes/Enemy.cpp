#include "Enemy.h"

#include "Algorithms.h"

namespace Retry {

Enemy::Enemy(const std::string& path, const cocos2d::Vec2& pos)
	: Actor(path, pos) {

}

GoonEnemy::GoonEnemy(const cocos2d::Vec2& pos)
	: Enemy("goon.png", pos) {

	Attack* atk = new Attack();
	atk->setDamage(10);
	atk->setDelay(0.0f);
	atk->setDuration(0.3f);
	atk->setRecovery(0.05f);
	atk->setKnockBackAmount(3);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCapsule(Vec2(40, 46), cocos2d::Vec2(52, 46), 10);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCH1] = atk;

	atk = new Attack();
	atk->setDamage(10);
	atk->setDelay(0.1f);
	atk->setDuration(0.3f);
	atk->setRecovery(0.15f);
	atk->setKnockBackAmount(3);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCapsule(Vec2(40, 46), cocos2d::Vec2(52, 46), 10);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCH2] = atk;

	atk = new Attack();
	atk->setDamage(20);
	atk->setDelay(0.0f);
	atk->setDuration(0.4f);
	atk->setRecovery(0.05f);
	atk->setKnockBackAmount(3);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCapsule(Vec2(40, 46), cocos2d::Vec2(51, 46), 25);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCHPUNCH] = atk;

	0.125 * (0.08 / 0.125);

	initAnimation("idle", "goon.png", Vec2(0, 0), Vec2(64, 64), 5);
	initAnimation("punch1", "goon.png", Vec2(0, 1), Vec2(64, 64), 5);
	initAnimation("punch2", "goon.png", Vec2(0, 2), Vec2(64, 64), 5);
	initAnimation("punchpunch", "goon.png", Vec2(0, 3), Vec2(64, 64), 6);

	runAnimation("idle", 0.1f);

	hitBox.addCapsule(Vec2(32, 16), Vec2(32, 48), 16);
	hitBox.addCapsule(Vec2(32, 16), Vec2(32, 48), 16);
}

TutorialEnemy::TutorialEnemy(const cocos2d::Vec2& pos)
	: GoonEnemy(pos) {
}

void Enemy::update(const float delta) {
	acceleration = cocos2d::Vec2(0, -2 * maxJumpHeight / (timeToMaxJumpHeight * timeToMaxJumpHeight));

	updateActionBuffer(delta);

	performSideMovement(delta);

	performJump(delta);

	moveBy(velocity * delta + 0.5f * acceleration * delta * delta);

	velocity.y += (!isActionPressed("jump") || velocity.y < 0 ? 2 : 1) * acceleration.y * delta;
	velocity.y = clamp(velocity.y, -2000, 2000);

	performAttack(delta);

	updateAnimations(delta);

	invincibilityTimer -= delta;
	attackTimer -= delta;
}

void GoonEnemy::update(const float delta) {

}

void TutorialEnemy::update(const float delta) {

}

void GoonEnemy::updateAI(const Player* player, const float delta) {

}

void TutorialEnemy::updateAI(const Player* player, const float delta) {

}

}