#pragma once
#include "cocos2d.h"

#include "CollisionBody.h"

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

enum AttackKey : char {
	PUNCH1 = 0x01,
	KICK1 = 0x02,
	PUNCH2 = 0x04,
	KICK2 = 0x08,
	PUNCHPUNCH = 0x10,
	PUNCHKICK = 0x20,
	KICKPUNCH = 0x40,
	KICKKICK = 0x80,
	FALLPUNCH = 0x81,
	FALLKICK = 0x82
};

class Attack;

class Actor {
public:
	Actor() = default;
	Actor(const std::string& path, const cocos2d::Vec2& position = { 0, 0 });
	virtual ~Actor();
	void kill(float delay = 0.0f);

	virtual void update(const float dt) = 0;

	void moveBy(cocos2d::Vec2 movement);

	void initAnimation(std::string action, std::string file, cocos2d::Vec2 startCell, cocos2d::Vec2 frameSize, int numFrames);

	void runAnimation(std::string action, float totalTime);

	void bufferAction(const std::string& action);

	virtual bool doTerrainCollision(Retry::Level* level, const float delta);

	bool isAttackCollidingWith(Actor* body);
	virtual void doAttackOnActor(Actor* actor);

	void adjustHealth(const float amount);

protected:
	virtual void performSideMovement(const float delta);

	virtual void performJump(const float delta);
	virtual void updateAnimations(const float delta);

	virtual void performAttack(const float delta);

	virtual void updateActionBuffer(const float delta);

	bool isActionPressed(const std::string &action);
	bool isActionDown(const std::string &action);
	bool isActionUp(const std::string &action);

	float actionPressedDuration(const std::string &action);
	float actionPressedValue(const std::string &action);

	float doSolidCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox);
	float doSolidCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox);

	float doPlatformCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox);
	float doPlatformCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox);

	float health = 100;
	float maxHealth = 100;

	float maxJumpHeight = 400;
	float timeToMaxJumpHeight = 0.65f;

	float sideMoveSpeed = 250;
	float timeToMaxSpeed = 0.4f;

	int doJump = 0;
	bool onGround = false;
	bool hasLanded = false;
	bool hasMoved = false;

	std::unordered_map<std::string, ActionInfo> actionBuffer;

	float invincibilityTimer = 0;

	float followUpAttackWindow = 0.3f;

	char currentAttackKey = 0;
	std::unordered_map<char, Attack*> attackList;

	float attackTimer = 0;

	float deathTimer = 0.1f;

	cocos2d::Sprite* sprite;
	cocos2d::Vec2 position;
	cocos2d::Vec2 velocity;
	cocos2d::Vec2 acceleration;

	cocos2d::Vec2 lastPosition;
	cocos2d::Vec2 deltaPosition;

	float totalTime;
	std::string currentAnimation;
	std::unordered_map<std::string, cocos2d::Animation*> animations;

	Collision::Body hitBox;
	cocos2d::Rect currentWorldHitBox;

	friend class Player;

public:
	cocos2d::Sprite* getSprite() { return sprite; }
	Retry::Collision::Body* getHitBox() { return &hitBox; }
	const cocos2d::Rect& getCurrentWorldHitBox() const { return currentWorldHitBox; }

	float getWidth() { return sprite->getBoundingBox().size.width; }
	float getHeight() { return sprite->getBoundingBox().size.height; }

	cocos2d::Vec2 getPosition() { return position; }
	cocos2d::Vec2 getVelocity() { return velocity; }
	cocos2d::Vec2 getAcceleration() { return acceleration; }

	void setPosition(const cocos2d::Vec2 &position) {
		lastPosition = this->position;

		sprite->setPosition(position + sprite->getBoundingBox().size / 2);
		this->position = position;

		deltaPosition = this->position - this->lastPosition;
	}
	void setVelocity(cocos2d::Vec2 velocity) { this->velocity = velocity; }
	void setAcceleration(cocos2d::Vec2 accel) { this->acceleration = accel; }

	float getMaxJumpHeight() const { return maxJumpHeight; }
	float getTimeToMaxJumpHeight() const { return timeToMaxJumpHeight; }
	void setMaxJumpHeigh(const float& f) { maxJumpHeight = f; }
	void setTimeTomaxJumpHeight(const float& f) { timeToMaxJumpHeight = f; }

	float getSideMoveSpeed() const { return sideMoveSpeed; }
	float getTimeToMaxSpeed() const { return timeToMaxSpeed; }
	void setSideMoveSpeed(const float& f) { sideMoveSpeed = f; }
	void setTimeToMaxSpeed(const float& f) { timeToMaxSpeed = f; }

	int getHealth() const { return (int) health; }
	int getMaxHealth() const { return (int) maxHealth; }
	float getHealthRatio() const { return health / maxHealth; }

	char getCurrentAttackKey() { return currentAttackKey; }
	Attack* getCurrentAttack() const { return attackList.find(currentAttackKey) != attackList.end() ? attackList.at(currentAttackKey) : nullptr; }

	bool isInvincible() const { return invincibilityTimer > 0; }
	void setInvincible(const float time) { invincibilityTimer = time; }

	void setFlippedX(const bool flip);

	bool shouldBeRemoved() { return deathTimer <= 0; }
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
	float getHitStun() const { return hitStun; }
	std::string getSoundFile() const { return soundFile; }
	Retry::Collision::Body* getHitBox() const { return &hitBox; }

	void setDamage(const float damage) { this->damage = damage; }
	void setKnockBackAmount(const float kbAmt) { this->kbAmt = kbAmt; }
	void setKnockBackDirection(const cocos2d::Vec2& kbDir) { this->kbDir = cocos2d::Vec2(abs(kbDir.x), kbDir.y).getNormalized(); }
	void setDelay(const float delay) { this->delay = delay; }
	void setDuration(const float duration) { this->duration = duration; }
	void setRecovery(const float recovery) { this->recovery = recovery; }
	void setHitStun(const float hitStun) { this->hitStun = hitStun; }
	void setSoundFile(const std::string& soundFile) { this->soundFile = soundFile; }

private:
	float damage = 0;

	float kbAmt = 0;
	cocos2d::Vec2 kbDir = Vec2::ZERO;

	float delay = 0.1f;
	float duration = 0.5f;
	float recovery = 0.2f;

	float hitStun = 0.0f;

	std::string soundFile = "";

	mutable Collision::Body hitBox;
};

class MenuActor : public Actor {
public:
	MenuActor(const std::string& path, const cocos2d::Vec2& position, const cocos2d::Size& frameSize, const int numFrames);

	virtual void update(const float delta) override;
};

}