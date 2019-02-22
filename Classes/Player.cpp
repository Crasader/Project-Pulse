#include "Player.h"

#include "Algorithms.h"

#include "GameSettings.h"

#include "CameraManager.h"

namespace Retry {

Retry::Player::Player(std::string path, cocos2d::Vec2 pos) {
	init(path, pos);

	addButtonToMapping("jump", KeyCode::SPACE);
	addButtonToMapping("jump", KeyCode::UP_ARROW);
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
	atk->setDelay(0.0f);
	atk->setDuration(0.3f);
	atk->setRecovery(0.05f);
	atk->getHitBox()->addRect(Vec2(64, 64), cocos2d::Size(200, 50));
	atk->getHitBox()->setParent(sprite);
	attackList[0b01] = atk;

	atk = new Attack();
	atk->setDelay(0.1f);
	atk->setDuration(0.3f);
	atk->setRecovery(0.1f);
	atk->getHitBox()->addCapsule(Vec2(64, 64), Vec2(128, 64), 32);
	atk->getHitBox()->setParent(sprite);
	attackList[0b100] = atk;

	atk = new Attack();
	atk->setDelay(0.0f);
	atk->setDuration(0.5f);
	atk->setRecovery(1.0f);
	atk->getHitBox()->addCapsule(Vec2(96, 64), Vec2(128, 64), 16);
	atk->getHitBox()->setParent(sprite);
	attackList[0b10000] = atk;
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
}

void Player::updateAnimations(const float& delta) {

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

void Player::performSideMovement(const float & delta) {

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

void Player::performJump(const float & delta) {
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


}