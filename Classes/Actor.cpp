#include "Actor.h"

#include "CameraManager.h"
#include "ControllerManager.h"
#include "KeyboardManager.h"
#include "GameSettings.h"
#include "Algorithms.h"

namespace Retry {

Actor::Actor(const std::string &name, const cocos2d::Vec2 &position) {
	init(name, position);
}

void Actor::setFlippedX(const bool& flip) {
	if (sprite->isFlippedX() == flip) return;

	sprite->setFlippedX(flip);
	hitBox.redraw();
	hurtBox.redraw();
	for (auto i : attackList) i.second->getHitBox()->redraw();
}

void Actor::update(const float& delta) {
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

bool Actor::doTerrainCollision(Retry::Level* level, const float &delta) {

	if (velocity.y < -800) onGround = false;

	cocos2d::Rect boundingBox(sprite->convertToWorldSpace(hurtBox.getBoundingBox().origin), hurtBox.getBoundingBox().size);
	for (cocos2d::Node* n = sprite; n != nullptr; n = n->getParent())
		boundingBox.size = boundingBox.size * n->getScale();

	doSolidCollisionX(level, boundingBox);
	doSolidCollisionY(level, boundingBox);

	doPlatformCollisionX(level, boundingBox);
	doPlatformCollisionY(level, boundingBox);

	// Update the sprite's position along with the character's position
	setPosition(position);

	return position != lastPosition;
}

bool Actor::isAttackCollidingWith(Actor* target) {
	Attack* currentAttack = getCurrentAttack();
	if (currentAttack != nullptr) {
		if (attackTimer > currentAttack->getRecovery() + currentAttack->getDuration() ||
			attackTimer < currentAttack->getRecovery())
			return false;

		return (invincibilityTimer <= 0 &&
				currentAttack->getHitBox()->isCollidingWith(target->getHurtBox()));
	}
}

void Actor::updateAnimations(const float & delta) {
	if (velocity.x != 0) {
		setFlippedX(velocity.x < 0);
	}
	if (onGround) {
		if (velocity.x != 0) {
			runAnimation("run", 0.077f);
		} else {
			runAnimation("idle", 0.12f);
		}
	} else {
		runAnimation("jump", 0.1f);
	}
}

void Actor::performAttack(const float& delta) {

	Attack* currAttack = getCurrentAttack();
	if (currAttack != nullptr) {
		if (attackTimer <= currAttack->getRecovery() + currAttack->getDuration() &&
			attackTimer >= currAttack->getRecovery())
			currAttack->getHitBox()->setDebugDraw(true);
		else currAttack->getHitBox()->setDebugDraw(false);
	}

	if (!isActionPressed("punch") && !isActionPressed("kick")) return;
	if (attackTimer <= -followUpAttackWindow) currentAttackKey = 0;

	bool isKick = isActionPressed("kick");

	// Each bit in "currentAttackKey" represents a different attack
	// 00000000
	// │││││││└ Punch 1
	// ││││││└─ Kick  1
	// │││││└── Punch 2
	// ││││└─── Kick  2
	// │││└──── Punch Punch Finisher
	// ││└───── Punch Kick  Finisher
	// │└────── Kick  Punch Finisher
	// └─────── Kick  Kick  Finisher
	if (attackTimer <= 0) {
		if (!currentAttackKey || currentAttackKey & 0b11110000) {
			currentAttackKey = 0b1 << (char) isKick;
		} else if (currentAttackKey & 0b11) {
			currentAttackKey = 0b100 << (char) isKick;
		} else if (currentAttackKey & 0b1100) {
			char temp = (currentAttackKey >> 2) + 1;
			currentAttackKey = 0b10000 << ((temp % 2) * 2 + (char) isKick);
		}

		currAttack = getCurrentAttack();
		if (currAttack != nullptr)
			attackTimer = currAttack->getDelay() + currAttack->getDuration() + currAttack->getRecovery();
	}
}

void Actor::performSideMovement(const float & delta) {
	if (isActionPressed("left") != isActionPressed("right")) {
		acceleration.x = (isActionPressed("left") ? -1 : 1) * sideMoveSpeed / timeToMaxSpeed / (!onGround ? (velocity.y > 100 ? 5.0f : 3.0f) : 1);
	} else {
		acceleration.x = !onGround ? 0 : -sign(velocity.x) * sideMoveSpeed / timeToMaxSpeed;
		if (sign(velocity.x) * (velocity.x + acceleration.x * delta) < 0)
			acceleration.x = velocity.x = 0;
	}
	velocity.x += acceleration.x * delta;
	velocity.x = sign(velocity.x) * clamp(abs(velocity.x), 0, sideMoveSpeed + (doJump ? 100 : 0));
}

void Actor::performJump(const float & delta) {
	if (onGround || Retry::Config::doDebug()) doJump = 0;
	if (doJump < 2 && isActionDown("jump")) {
		onGround = false;
		velocity.y = 2 * maxJumpHeight / timeToMaxJumpHeight;
		doJump++;

		if (isActionPressed("left") && velocity.x > 0 || isActionPressed("right") && velocity.x < 0) {
			velocity.x *= -0.5f;
		} else if (!isActionPressed("left") && !isActionPressed("right")) {
			velocity.x *= 0.2f;
		}
	}
}

// TENTATIVE FUNCTIONS DO NOT USE UNLESS YOU KNOW WHAT THEY DO
void Actor::decreaseHealth(const float &delta) {
	health -= 40 * delta;
	if (health < 0) health = 0;
}

void Actor::increaseHealth(const float &delta) {
	health += 40 * delta;
	if (health > maxHealth) health = maxHealth;
}

void Actor::updateActionBuffer(const float& delta) {
	for (auto& i : actionBuffer) {
		if (i.second.time != 0) {
			i.second.down = i.second.value == 0;
			i.second.value = 1;
		} else {
			i.second.up = i.second.value != 0;
			i.second.value = 0;
		}
		i.second.time = 0;
	}
}

void Actor::bufferAction(const std::string &action) {

	actionBuffer[action].time = 1;
}

bool Actor::isActionPressed(const std::string &action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].value != 0;
	return false;
}

bool Actor::isActionDown(const std::string &action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].down;
	return false;
}

bool Actor::isActionUp(const std::string &action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].up;
	return false;
}

float Actor::actionPressedDuration(const std::string &action) {
	//if (actionBuffer.find(action) != actionBuffer.end())
	//	return actionBuffer[action].time;
	return 0;
}

float Actor::actionPressedValue(const std::string &action) {
	if (actionBuffer.find(action) != actionBuffer.end())
		return actionBuffer[action].value;
	return 0;
}

float Actor::doSolidCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox) {
	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));
	float incY = (boundingBox.getMaxY() - boundingBox.getMinY()) / ceil(getHeight() / (level->getTileSize() - 1));
	if (!incX || !incY) return position.x;

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX) {
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m; j += incY) {
			Vec2 currentTile = Vec2(i, j - deltaPosition.y) / level->getTileSize();
			if (boundingBox.getMinX() < 0 || level->getCollisionDataAt(currentTile) & 0x01) {
				position.x = lastPosition.x;
				velocity.x = 0;

				return position.x;
			}
		}
	}
	return position.x;
}

float Actor::doSolidCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox) {
	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));
	float incY = (boundingBox.getMaxY() - boundingBox.getMinY()) / ceil(getHeight() / (level->getTileSize() - 1));
	if (!incX || !incY) return position.y;

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX) {
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m; j += incY) {
			Vec2 currentTile = Vec2(i - (position.x == lastPosition.x ? deltaPosition.x : 0), j) / level->getTileSize();
			if (boundingBox.getMinY() < 0 || level->getCollisionDataAt(currentTile) & 0x01) {
				if (!onGround && velocity.y < 0) {
					hasLanded = true;
					onGround = true;
				}

				position.y = lastPosition.y;
				velocity.y = 0;
				return position.y;
			}
		}
	}
	return position.y;
}

float Actor::doPlatformCollisionX(Retry::Level* level, const cocos2d::Rect &boundingBox) {


	return position.x;
}

float Actor::doPlatformCollisionY(Retry::Level* level, const cocos2d::Rect &boundingBox) {
	if (velocity.y > 0 ||
		Controller::getAxis(ControllerButton::LEFT_STICK_DOWN) < -0.8f ||
		Keyboard::isKeyDown(KeyCode::S)) return position.y;

	float incX = (boundingBox.getMaxX() - boundingBox.getMinX()) / ceil(getWidth() / (level->getTileSize() - 1));
	if (!incX) return position.y;

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n; i += incX) {
		Vec2 currentTile = Vec2(i - (position.x == lastPosition.x ? deltaPosition.x : 0), boundingBox.getMinY()) / level->getTileSize();
		if ((level->getCollisionDataAt(currentTile) & 0x02) &&
			!level->getCollisionDataAt(Vec2(currentTile.x, (boundingBox.getMinY() - deltaPosition.y) / level->getTileSize()))) {
			if (!onGround) {
				Camera::addTrauma(0.3f);
				Controller::vibrate(0.2f, 0.1f);
				onGround = true;
			}

			position.y = lastPosition.y;
			velocity.y = 0;
			return position.y;
		}
	}
	return position.y;
}

}