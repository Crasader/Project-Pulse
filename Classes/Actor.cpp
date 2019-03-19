#include "Actor.h"

#include "CameraManager.h"
#include "ControllerManager.h"
#include "KeyboardManager.h"
#include "GameSettings.h"
#include "Algorithms.h"

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

namespace Retry {

Actor::Actor(const std::string &path, const cocos2d::Vec2 &position) {

	sprite = cocos2d::Sprite::create(path);
	if (sprite == nullptr) {
		cocos2d::log("Sprite %s could not be loaded!", path.c_str());
		auto rt = cocos2d::RenderTexture::create(128, 128);
		rt->beginWithClear(1, 0, 1, 1);
		rt->end();
		sprite = cocos2d::Sprite::createWithTexture(rt->getSprite()->getTexture());
	}
	sprite->getTexture()->setAliasTexParameters();
	sprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	sprite->setPosition(position);
	this->position = position;

	new (&hitBox)  Retry::Collision::Body(sprite);
	hitBox.setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
}

Actor::~Actor() {
	for (auto i : attackList)
		delete i.second;
}

void Actor::kill(float delay) {
	sprite->runAction(cocos2d::RemoveSelf::create());
}

void Actor::moveBy(cocos2d::Vec2 movement) {
	setPosition(position + movement);
	//position += movement;
	//sprite->setPosition(position);
}

void Actor::initAnimation(std::string action, std::string file, Vec2 startCell, Vec2 frameSize, int numFrames) {
	//TODO: Add more functionality
	auto temp = cocos2d::Animation::create();
	int width = sprite->getTexture()->getPixelsWide() / frameSize.x;
	for (int i = 0; i < numFrames; i++) {
		Vec2 startPosition((((int) startCell.x + i) % width) * frameSize.x, (startCell.y + ((int) startCell.x + i) / width) * (frameSize.y) + 2);
		auto frame = cocos2d::SpriteFrame::create(file.c_str(), Rect(startPosition, Size(frameSize) - Size(0, 2)));
		//frame->getTexture()->setAliasTexParameters();
		temp->addSpriteFrame(frame);
	}
	temp->setLoops(1);

	animations[action] = temp;
	animations[action]->retain();
}

void Actor::runAnimation(std::string action, float totalTime) {
	if (animations.find(action) == animations.end()) return;

	auto currentAnimate = (cocos2d::Animate*)sprite->getActionByTag('anim');

	if (totalTime > 0) animations[action]->setDelayPerUnit(totalTime);

	if (currentAnimation != action) {
		currentAnimation = action;
		sprite->stopAllActionsByTag('anim');
		auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
		tempAnim->setTag('anim');
		sprite->runAction(tempAnim);
	} else if (currentAnimate == nullptr) {
		sprite->stopAllActionsByTag('anim');
		auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
		tempAnim->setTag('anim');
		sprite->runAction(tempAnim);
	} else {
		float elapsed = currentAnimate->getElapsed();
		if (this->totalTime != totalTime && elapsed > 0) {
			sprite->stopAllActionsByTag('anim');
			auto tempAnim = cocos2d::Animate::create(animations[action]->clone());
			tempAnim->setTag('anim');
			sprite->runAction(tempAnim);
			((cocos2d::Animate*)sprite->getActionByTag('anim'))->step(elapsed);
		}
	}
	this->totalTime = totalTime;
	//cocos2d::log("%f", ((cocos2d::Animate*)sprite->getActionByTag('anim')));
}

void Actor::setFlippedX(const bool flip) {
	if (sprite->isFlippedX() == flip) return;

	sprite->setFlippedX(flip);
	hitBox.redraw();
	for (auto i : attackList) i.second->getHitBox()->redraw();
}

bool Actor::doTerrainCollision(Retry::Level* level, const float delta) {

	if (velocity.y < -800) onGround = false;

	Rect boundingBox = hitBox.getWorldBoundingBox();

	//cocos2d::Rect boundingBox = hurtBox.getBoundingBox();

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

		return (!target->isInvincible() &&
				currentAttack->getHitBox()->isCollidingWith(target->getHitBox()));
	} else return false;
}

void Actor::updateAnimations(const float delta) {
	float attackFrameLength = 0.125f * (0.08f / 0.125f);

	if (velocity.x != 0 && attackTimer < -this->followUpAttackWindow) {
		if (isActionPressed("left") && !isActionPressed("right"))
			setFlippedX(true);
		if (isActionPressed("right") && !isActionPressed("left"))
			setFlippedX(false);
	}
	if (attackTimer > -0.15) {
		switch (currentAttackKey) {
			case PUNCH1:
				runAnimation("punch1", attackFrameLength); break;
			case KICK1:
				runAnimation("kick1", attackFrameLength); break;
			case PUNCH2:
				runAnimation("punch2", attackFrameLength); break;
			case KICK2:
				runAnimation("kick2", attackFrameLength); break;
			case PUNCHPUNCH:
				runAnimation("punchpunch", attackFrameLength); break;
			case PUNCHKICK:
				runAnimation("punchkick", attackFrameLength); break;
			case KICKPUNCH:
				runAnimation("kickpunch", attackFrameLength); break;
			case KICKKICK:
				runAnimation("kickkick", attackFrameLength); break;
			case FALLPUNCH:
				runAnimation("fallpunch", attackFrameLength); break;
			case FALLKICK:
				runAnimation("fallkick", attackFrameLength); break;
		}
		return;
	}
	if (onGround) {
		if (velocity.x != 0) {
			//runAnimation("run", 0.077f);
		} else {
			runAnimation("idle", 0.12f);
		}
	} else {
		//if (velocity.y > 0)
		//	runAnimation("jump", 0.1f);
		//else
		//	runAnimation("fall", 0.1f);
	}
}

void Actor::performAttack(const float delta) {

	Attack* currAttack = getCurrentAttack();
	if (currAttack != nullptr) {
		// HACK: remove for demo
		//if (attackTimer <= currAttack->getRecovery() + currAttack->getDuration() &&
		//	attackTimer >= currAttack->getRecovery())
		//	currAttack->getHitBox()->setDebugDraw(true);
		//else currAttack->getHitBox()->setDebugDraw(false);
	}

	if (attackTimer <= -followUpAttackWindow) currentAttackKey = 0;
	if (!isActionDown("punch") && !isActionDown("kick")) return;

	bool isKick = isActionDown("kick");

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
			currentAttackKey = (0b1 << (char) isKick) | (!onGround ? 0x80 : 0);
		} else if (onGround && (currentAttackKey & 0b11)) {
			currentAttackKey = 0b100 << (char) isKick;
		} else if (onGround && (currentAttackKey & 0b1100)) {
			char temp = (currentAttackKey >> 2) + 1;
			currentAttackKey = 0b10000 << ((temp % 2) * 2 + (char) isKick);
		}
		if (currentAttackKey) {
			if (isActionPressed("left") && !isActionPressed("right"))
				setFlippedX(true);
			else if (isActionPressed("right") && !isActionPressed("left"))
				setFlippedX(false);
		}

		currAttack = getCurrentAttack();
		if (currAttack != nullptr)
			attackTimer = currAttack->getDelay() + currAttack->getDuration() + currAttack->getRecovery();
	}
}

void Actor::performSideMovement(const float delta) {
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

void Actor::performJump(const float delta) {
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

void Actor::adjustHealth(const float amount) {
	this->health += amount;
	this->health = clamp(this->health, 0, this->maxHealth);
}

void Actor::doAttackOnActor(Actor* actor) {
	if (getCurrentAttack()) {
		actor->adjustHealth(-abs(getCurrentAttack()->getDamage()));

		Vec2 kb = getCurrentAttack()->getKnockBackDirection() * getCurrentAttack()->getKnockBackAmount() * 100;
		kb.x = sprite->isFlippedX() ? -kb.x : kb.x;
		if (kb.y > 0) {
			actor->doJump++;
			actor->onGround = false;
		}
		actor->setVelocity(kb);

		actor->currentAttackKey = 0;
		actor->attackTimer = -actor->followUpAttackWindow - 0.1f;

		actor->invincibilityTimer = getCurrentAttack()->getDuration();
	}

}

void Actor::updateActionBuffer(const float delta) {
	for (auto& i : actionBuffer) {
		i.second.down = i.second.time == 1 && i.second.value == 0;
		i.second.up = i.second.time == 0 && i.second.value == 1;

		i.second.value = i.second.time;
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

	if (boundingBox.getMinX() < Camera::getMinX() || boundingBox.getMaxX() > Camera::getMinX())

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n + incX * 0.5f; i += incX) {
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m + incY * 0.5f; j += incY) {
			Vec2 currentTile = Vec2(i, j - deltaPosition.y) / level->getTileSize();
			if (boundingBox.getMinX() < Camera::getMinX() || 
				boundingBox.getMaxX() > Camera::getMaxX() || 
				level->getCollisionDataAt(currentTile) & 0x01) {
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

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n + incX * 0.5f; i += incX) {
		for (float j = boundingBox.getMinY(), m = boundingBox.getMaxY(); j <= m + incY * 0.5f; j += incY) {
			Vec2 currentTile = Vec2(i - (position.x == lastPosition.x ? deltaPosition.x : 0), j) / level->getTileSize();
			// HACK: Not supposed to be Camera::getMinY()
			if (boundingBox.getMinY() < Camera::getMinY() || level->getCollisionDataAt(currentTile) & 0x01) {
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

	for (float i = boundingBox.getMinX(), n = boundingBox.getMaxX(); i <= n + incX * 0.5f; i += incX) {
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