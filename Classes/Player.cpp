#include "Player.h"

#include "Algorithms.h"

#include "GameSettings.h"

#include "CameraManager.h"

namespace Retry {

Retry::Player::Player(std::string path, cocos2d::Vec2 pos) {
	init(path, pos);

	addButtonToMapping("jump", KeyCode::SPACE);
	addButtonToMapping("jump", KeyCode::W);
	addButtonToMapping("jump", ControllerButton::A);

	addButtonToMapping("left", KeyCode::A);
	addButtonToMapping("left", ControllerButton::LEFT_STICK_LEFT);

	addButtonToMapping("right", KeyCode::D);
	addButtonToMapping("right", ControllerButton::LEFT_STICK_RIGHT);

	addButtonToMapping("leftStick", ControllerButton::LEFT_STICK_X);

	addButtonToMapping("punch", KeyCode::K);
	addButtonToMapping("punch", ControllerButton::X);

	addButtonToMapping("kick", KeyCode::L);
	addButtonToMapping("kick", ControllerButton::Y);


	// HACK: Read in data from file maybe?
	Attack* atk = new Attack();
	atk->setDamage(10);
	atk->setDelay(0.3f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.1f * (0.08 / 0.125));
	atk->setKnockBackAmount(1);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCapsule(Vec2(40, 46), cocos2d::Vec2(61, 46), 15);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCH1] = atk;


	atk = new Attack();
	atk->setDamage(15);
	atk->setDelay(0.3f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.2f * (0.08 / 0.125));
	atk->setKnockBackAmount(5);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCircle(Vec2(48, 12), 20);
	atk->getHitBox()->setParent(sprite);
	attackList[KICK1] = atk;

	atk = new Attack();
	atk->setDamage(12);
	atk->setDelay(0.3f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.1f * (0.08 / 0.125));
	atk->setKnockBackAmount(1);
	atk->setKnockBackDirection(Vec2(1, 0.25f));
	atk->getHitBox()->addCapsule(Vec2(40, 46), cocos2d::Vec2(61, 46), 15);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCH2] = atk;

	atk = new Attack();
	atk->setDamage(18);
	atk->setDelay(0.3f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.2f * (0.08 / 0.125));
	atk->setKnockBackAmount(4);
	atk->setKnockBackDirection(Vec2(1, 0.5f));
	atk->getHitBox()->addCircle(Vec2(46, 36), 30);
	atk->getHitBox()->setParent(sprite);
	attackList[KICK2] = atk;

	atk = new Attack();
	atk->setDamage(20);
	atk->setDelay(0.45f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.4f * (0.08 / 0.125));
	atk->setKnockBackAmount(4);
	atk->setKnockBackDirection(Vec2(0.25, 1));
	atk->getHitBox()->addRect(Vec2(24, 32), cocos2d::Size(100, 45));
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCHPUNCH] = atk;

	atk = new Attack();
	atk->setDamage(25);
	atk->setDelay(0.3f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.3f * (0.08 / 0.125));
	atk->setKnockBackAmount(8);
	atk->setKnockBackDirection(Vec2(1, 1));
	atk->getHitBox()->addCapsule(Vec2(64, 64), Vec2(128, 64), 24);
	atk->getHitBox()->setParent(sprite);
	attackList[PUNCHKICK] = atk;

	atk = new Attack();
	atk->setDamage(30);
	atk->setDelay(0.45f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.3f * (0.08 / 0.125));
	atk->setKnockBackAmount(10);
	atk->setKnockBackDirection(Vec2(1, 1));
	atk->getHitBox()->addCapsule(Vec2(32, 64), cocos2d::Size(112, 64), 32);
	atk->getHitBox()->setParent(sprite);
	attackList[KICKPUNCH] = atk;

	atk = new Attack();
	atk->setDamage(25);
	atk->setDelay(0.55f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.3f * (0.08 / 0.125));
	atk->setKnockBackAmount(15);
	atk->setKnockBackDirection(Vec2(1, 1));
	atk->getHitBox()->addCircle(Vec2(64, 64), 48);
	atk->getHitBox()->setParent(sprite);
	attackList[KICKKICK] = atk;

	atk = new Attack();
	atk->setDamage(25);
	atk->setDelay(0.55f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.3f * (0.08 / 0.125));
	atk->setKnockBackAmount(15);
	atk->setKnockBackDirection(Vec2(1, 1));
	atk->getHitBox()->addCircle(Vec2(64, 64), 48);
	atk->getHitBox()->setParent(sprite);
	attackList[FALLPUNCH] = atk;

	atk = new Attack();
	atk->setDamage(25);
	atk->setDelay(0.55f * (0.08 / 0.125));
	atk->setDuration(0.1f * (0.08 / 0.125));
	atk->setRecovery(0.3f * (0.08 / 0.125));
	atk->setKnockBackAmount(15);
	atk->setKnockBackDirection(Vec2(1, 1));
	atk->getHitBox()->addCircle(Vec2(64, 64), 48);
	atk->getHitBox()->setParent(sprite);
	attackList[FALLKICK] = atk;

	const cocos2d::Vec2 tileSize(64, 64);

	initAnimation("run", "cybercop.png", cocos2d::Vec2(0, 2), tileSize, 6);
	initAnimation("idle", "cybercop.png", cocos2d::Vec2(0, 0), tileSize, 20);
	initAnimation("jump", "cybercop.png", cocos2d::Vec2(6, 2), tileSize, 14);
	initAnimation("fall", "cybercop.png", Vec2(6, 4), tileSize, 4);

	initAnimation("punch1", "cybercop.png", Vec2(0, 6), tileSize, 5);
	initAnimation("punch2", "cybercop.png", Vec2(5, 6), tileSize, 5);

	initAnimation("kick1", "cybercop.png", Vec2(0, 5), tileSize, 6);
	initAnimation("kick2", "cybercop.png", Vec2(0, 4), tileSize, 6);

	initAnimation("punchpunch", "cybercop.png", Vec2(0, 7), tileSize, 8);
	initAnimation("punchkick", "cybercop.png", Vec2(0, 8), tileSize, 6);
	initAnimation("kickpunch", "cybercop.png", Vec2(0, 9), tileSize, 7);
	initAnimation("kickkick", "cybercop.png", Vec2(0, 10), tileSize, 8);

	initAnimation("fallpunch", "cybercop.png", Vec2(5, 11), tileSize, 5);
	initAnimation("fallkick", "cybercop.png", Vec2(0, 11), tileSize, 5);

	runAnimation("run", 0);

	hurtBox.addCapsule(Vec2(32, 16), Vec2(32, 48), 16);

	sprite->setScale(2);

	sideMoveSpeed = 700;
	timeToMaxSpeed = 0.1f;
}

void Retry::Player::update(const float& delta) {
	acceleration = cocos2d::Vec2(0, -2 * maxJumpHeight / (timeToMaxJumpHeight * timeToMaxJumpHeight));

	updateActionBuffer(delta);

	performSideMovement(delta);

	performJump(delta);

	if (hasLanded) {
		Retry::Camera::addTrauma(0.3f);
		Controller::vibrate(0.2f, 0.1f);
		hasLanded = false;
	}

	moveBy(velocity * delta + 0.5f * acceleration * delta * delta);

	velocity.y += (!isActionPressed("jump") || velocity.y < 0 ? 2 : 1) * acceleration.y * delta;
	velocity.y = clamp(velocity.y, -2000, 2000);

	performAttack(delta);

	updateAnimations(delta);

	invincibilityTimer -= delta;
	attackTimer -= delta;
	if (pulseCooldown > 0) pulseCooldown -= delta;
	else if (health < maxHealth * 0.15f) isPulse = false;

	if (Keyboard::isKeyDown(KeyCode::SHIFT) || Controller::isButtonDown(ControllerButton::B))
		isPulse = !isPulse;
}

void Player::updateAnimations(const float& delta) {

	float attackFrameLength = 0.125 * (0.08 / 0.125);

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
			runAnimation("run", 0.077f);
		} else {
			runAnimation("idle", 0.12f);
		}
	} else {
		if (velocity.y > 0)
			runAnimation("jump", 0.1f);
		else
			runAnimation("fall", 0.1f);
	}

}

bool Player::doTerrainCollision(Level * level, const float & delta) {

	if (velocity.y < -800) onGround = false;

	cocos2d::Rect boundingBox(sprite->convertToWorldSpace(hurtBox.getBoundingBox().origin), hurtBox.getBoundingBox().size);
	for (cocos2d::Node* n = sprite; n != nullptr; n = n->getParent())
		boundingBox.size = boundingBox.size * n->getScale();
	boundingBox.origin = boundingBox.origin / cocos2d::Director::getInstance()->getRunningScene()->getScale();
	boundingBox.size = boundingBox.size / cocos2d::Director::getInstance()->getRunningScene()->getScale();

	//cocos2d::Rect boundingBox = hurtBox.getBoundingBox();

	doSolidCollisionX(level, boundingBox);
	doSolidCollisionY(level, boundingBox);

	doPlatformCollisionX(level, boundingBox);
	doPlatformCollisionY(level, boundingBox);

	if (canMoveOn)
		doCameraCollision(level, boundingBox);
	canMoveOn = true;

    // Update the sprite's position along with the character's position
	setPosition(position);

	return position != lastPosition;
}

float Player::doCameraCollision(Retry::Level* level, const cocos2d::Rect &boundingBox) {
	Vec2 currentTile = (boundingBox.origin + Vec2(0, boundingBox.size.height / 2)) / level->getTileSize();
	if (level->getCollisionDataAt(currentTile) & 0x80) {
		Vec3 tileData = level->getTileDataAt(currentTile);
		Camera::setMinX((currentTile - Vec2(0.5f, 0)).x * level->getTileSize());
		Camera::setMinY(tileData.y * level->getTileSize() * 2);
		Camera::setMaxX(Camera::getMinX() + tileData.x * level->getTileSize() * 2);
		//Camera::setMaxY(Camera::getMinY() + tileData.z * level->getTileSize() * 2);
	}
	return position.x;
}

void Player::performSideMovement(const float & delta) {

	sideMoveSpeed = getMode() == COOLDOWN ? 400 : 700;

	bool midAttack = attackTimer > -followUpAttackWindow && onGround;

	if ((isActionPressed("left") != isActionPressed("right")) && !midAttack) {
		acceleration.x = (isActionPressed("left") ? -1 : 1) * sideMoveSpeed / timeToMaxSpeed / (!onGround ? (velocity.y > 100 ? 5.0f : 3.0f) : 1);
	} else {
		acceleration.x = !onGround ? 0 : -sign(velocity.x) * sideMoveSpeed / (!midAttack ? timeToMaxSpeed : 0.5f);
		if (sign(velocity.x) * (velocity.x + acceleration.x * delta) < 0)
			acceleration.x = velocity.x = 0;
	}
	velocity.x += acceleration.x * delta;
	velocity.x = sign(velocity.x) * clamp(abs(velocity.x), 0, sideMoveSpeed + (doJump ? 100 : 0));
}

void Player::performJump(const float & delta) {
	if (onGround || Retry::Config::doDebug()) doJump = 0;
	if (doJump < 2 && attackTimer < -0.15 && isActionDown("jump")) {
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

void Player::updatePulseMode(const float & delta) {






}

void Retry::Player::updateActionBuffer(const float& delta) {
	for (auto &i : actionBuffer) {
		float time = 0;
		int count = 0;
		i.second.down = false;
		i.second.up = false;
		i.second.value = 0;

		for (auto j : actionMapping[i.first].kButtons) {
			i.second.down = i.second.down || Retry::Keyboard::isKeyDown(j);
			i.second.up = i.second.up || Retry::Keyboard::isKeyUp(j);
			i.second.value = i.second.value || Retry::Keyboard::isKeyPressed(j);
		}
		for (auto j : actionMapping[i.first].mButtons) {
			i.second.down = i.second.down || Retry::Mouse::isButtonDown(j);
			i.second.up = i.second.up || Retry::Mouse::isButtonUp(j);
			i.second.value = i.second.value || Retry::Mouse::isButtonPressed(j);
		}
		for (auto j : actionMapping[i.first].cButtons) {
			if ((int) j < 16) {
				i.second.down = i.second.down || Retry::Controller::isButtonDown(j);
				i.second.up = i.second.up || Retry::Controller::isButtonUp(j);
				i.second.value = i.second.value || Retry::Controller::isButtonPressed(j);
			} else {
				i.second.down = i.second.down || Retry::Controller::isAxisDown(j);
				i.second.up = i.second.up || Retry::Controller::isAxisUp(j);
				float tVal = Retry::Controller::getAxis(j);
				i.second.value = tVal != 0 && Retry::Controller::isAxisPressed(j) ? tVal : i.second.value;
			}
		}
	}
}

void Player::addButtonToMapping(const std::string &action, const Retry::KeyCode &button) {
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].kButtons.push_back(button);
}

void Player::addButtonToMapping(const std::string &action, const Retry::MouseButton &button) {
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].mButtons.push_back(button);
}

void Player::addButtonToMapping(const std::string &action, const Retry::ControllerButton &button) {
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].cButtons.push_back(button);
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::KeyCode &button) {
	auto keyMap = &actionMapping[action].kButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++) {
		if (keyMap->at(i) == button) {
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::MouseButton &button) {
	auto keyMap = &actionMapping[action].mButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++) {
		if (keyMap->at(i) == button) {
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::ControllerButton &button) {
	auto keyMap = &actionMapping[action].cButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++) {
		if (keyMap->at(i) == button) {
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}

void Player::doAttackOnActor(Actor * actor) {
	if (getCurrentAttack()) {
		float damage = -abs(getCurrentAttack()->getDamage());

		Vec2 kb = getCurrentAttack()->getKnockBackDirection() * getCurrentAttack()->getKnockBackAmount() * 100;
		kb.x += velocity.x;
		kb.x = sprite->isFlippedX() ? -kb.x : kb.x;
		if (kb.y > 0) {
			actor->doJump++;
			actor->onGround = false;
		}

		Camera::setTrauma(0.15f);

		if (currentAttackKey & 0xF0) {
			if (getMode() == PULSE) {
				this->adjustHealth(damage * 0.5f);
				damage *= pulseDamageBoost;
				kb = kb * pulseKnockbackBoost;
				if (health < maxHealth * 0.15f)
					pulseCooldown = pulseCooldownTimer;
				Camera::setTrauma(0.6f);
				Controller::vibrate(0.9f, 0.1f);
			} else if (getMode() == REST) {
				this->adjustHealth(-damage * 0.5f);
				//if (health > maxHealth * 0.85f)
				//	pulseCooldown = pulseCooldownTimer;
				Camera::setTrauma(0.4f);
				Controller::vibrate(0.6f, 0.1f);
			}
		}

		actor->adjustHealth(damage);
		actor->setVelocity(kb);
		actor->invincibilityTimer = getCurrentAttack()->getDuration();
	}
}


}