#pragma once
#include "cocos2d.h"

#include "Entity.h"

#include "Level.h"

namespace {
struct ActionInfo {
	float time = 0;
	bool down = false;
	bool up = false;
	float value = 0;
};
}

namespace Retry {

class Attack;

class Actor : public Entity {
public:
	Actor() = default;
	Actor(const std::string &path, const cocos2d::Vec2 &position);
	Actor::~Actor() { delete sprite; }

	void bufferAction(const std::string &action);

	virtual void update(const float& delta) override;
	virtual bool doTerrainCollision(Retry::Level* level, const float &delta);

	bool isAttackCollidingWith(Actor* body);

	// GETTERS AND SETTERS
	float getMaxJumpHeight() { return maxJumpHeight; }
	float getTimeToMaxJumpHeight() { return timeToMaxJumpHeight; }
	void setMaxJumpHeigh(const float& f) { maxJumpHeight = f; }
	void setTimeTomaxJumpHeight(const float& f) { timeToMaxJumpHeight = f; }

	float getSideMoveSpeed() { return sideMoveSpeed; }
	float getTimeToMaxSpeed() { return timeToMaxSpeed; }
	void setSideMoveSpeed(const float& f) { sideMoveSpeed = f; }
	void setTimeToMaxSpeed(const float& f) { timeToMaxSpeed = f; }

	int getHealth() { return (int) health; }
	int getMaxHealth() { return (int) maxHealth; }
	float getHealthRatio() { return health / maxHealth; }
	void decreaseHealth(const float &delta);
	void increaseHealth(const float &delta);

	bool isInvincible() { return invincibilityTimer <= 0; }
	void setInvincible() { invincibilityTimer = invincibilityTime; }

protected:

	void setFlippedX(const bool& flip);

	virtual void performSideMovement(const float& delta);

	virtual void performJump(const float& delta);
	virtual void updateAnimations(const float& delta);

	virtual void performAttack(const float& delta);

	float health = 100;
	float maxHealth = 100;

	float maxJumpHeight = 400;
	float timeToMaxJumpHeight = 0.65f;

	float sideMoveSpeed = 700;
	float timeToMaxSpeed = 0.1f;

	int doJump = 0;
	bool onGround = false;
	bool hasLanded = false;
	bool hasMoved = false;

	std::unordered_map<std::string, cocos2d::Animation*> animations;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	virtual void updateActionBuffer(const float& delta);

	bool isActionPressed(const std::string &action);
	bool isActionDown(const std::string &action);
	bool isActionUp(const std::string &action);

	float actionPressedDuration(const std::string &action);
	float actionPressedValue(const std::string &action);

	float doSolidCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox);
	float doSolidCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox);

	float doPlatformCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox);
	float doPlatformCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox);

	float invincibilityTime = 0.3f;
	float invincibilityTimer = 0;

	float followUpAttackWindow = 0.4f;

	char currentAttackKey = 0;
	std::unordered_map<char, Attack*> attackList;

	Attack* getCurrentAttack() { return attackList.find(currentAttackKey) != attackList.end() ? attackList[currentAttackKey] : nullptr; }

	float attackTimer = 0;
};

class Attack {
public:
	Attack() = default;

	float getDamage() const { return damage; }
	float getKnockBackAmount() const { return kbAmt; }
	cocos2d::Vec2 getKnockBackDirection() const { return kbDir; }
	float getDelay() const { return delay; }
	float getDuration() const { return duration; }
	float getRecovery() const { return recovery; }
	Retry::Collision::Body* getHitBox() const { return &hitBox; }

	void setDamage(const float& dmg) { damage = dmg; }
	void setKnockBackAmount(const float& kbAmt) { this->kbAmt = kbAmt; }
	void setKnockBackDirection(const cocos2d::Vec2& kbDir) { this->kbDir = kbDir; }
	void setDelay(const float& delay) { this->delay = delay; }
	void setDuration(const float& duration) { this->duration = duration; }
	void setRecovery(const float& recovery) { this->recovery = recovery; }

private:
	float damage;

	float kbAmt;
	cocos2d::Vec2 kbDir;

	float delay;
	float duration;
	float recovery;

	mutable Collision::Body hitBox;
};

}