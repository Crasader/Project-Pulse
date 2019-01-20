#include "Player.h"

#include "CameraManager.h"

Retry::KeyboardManager* Player::keyIn = Retry::KeyboardManager::getInstance();
Retry::MouseManager* Player::mouseIn = Retry::MouseManager::getInstance();
Retry::ControllerManager* Player::controllerIn = Retry::ControllerManager::getInstance();
Retry::AudioManager* Player::audio = Retry::AudioManager::getInstance();

std::unordered_map<std::string, KeyMap> Player::actionMapping;

Player::Player(std::string path, Vec2 pos) {
	load(path, pos);

	actionBuffer["jump"];
	actionBuffer["left"];
	actionBuffer["right"];

	using namespace Retry;
	KeyMap jump;
	jump.kButtons.push_back(KeyCode::SPACE);
	jump.cButtons.push_back(ControllerButton::A);
	KeyMap left;
	left.kButtons.push_back(KeyCode::A);
	left.cButtons.push_back(ControllerButton::LEFT_STICK_LEFT);
	left.cButtons.push_back(ControllerButton::DPAD_LEFT);
	KeyMap right;
	right.kButtons.push_back(KeyCode::D);
	right.cButtons.push_back(ControllerButton::LEFT_STICK_RIGHT);
	right.cButtons.push_back(ControllerButton::DPAD_RIGHT);

	actionMapping["jump"] = jump;
	actionMapping["left"] = left;
	actionMapping["right"] = right;
}

float lerp(float p0, float p1, float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	return p1 * t + p0 * (1 - t);
}

float lerpSq(float p0, float p1, float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	return p1 * t * t + p0 * (1 - t * t);
}

int sign(float x) {
	return x < 0 ? -1 : x > 0 ? 1 : 0;
}

void Player::update(float delta) {
	updateActionBuffer();

	using namespace Retry;

	bool jumpButtonDown = isActionDown("jump");
	bool jumpButtonPressed = isActionPressed("jump");

	bool goLeft = isActionPressed("left"),
		goRight = isActionPressed("right");

	bool leftStickSens = controllerIn->isAxisPressed(ControllerButton::LEFT_STICK_LEFT) || 
		                 controllerIn->isAxisPressed(ControllerButton::LEFT_STICK_RIGHT);

	// Side Movement Constants and Variables
	static const float sideMove = 450;
	static const float timeToMax = 0.1f;
	static float time = 0;

	// Jumping Constants and Variables
	static const float h = 200;
	static const float t_h = 0.5f;
	static const float g = (-2 * h) / (t_h * t_h);
	static const float fastFall = 2.5f;
	static int doJump = 0;
	static bool hasMoved = false;
	static bool onGround = true;

	static Vec2 vel = Vec2(0, 0), acc = Vec2(0, g);

	// LANDING
	static const float groundHeight = 50;
	if (sprite->getPosition().y < groundHeight) {

		if (!onGround)
			CameraManager::getInstance()->addTrauma(0.3f);

		sprite->setPositionY(groundHeight);
		doJump = 0;
		hasMoved = false;
		onGround = true;
	}
	
	float step = (!doJump || goLeft || goRight) * (delta / timeToMax) / (doJump ? (vel.y > 100 ? 5.0f : 3.0f) : 1);

	if (goLeft || goRight) hasMoved = true;

	// MOVEMENT
	if (goLeft && !goRight) {
		time = time - step < -1 ? -1 : time - step;
	} else if (goRight && !goLeft) {
		time = time + step > 1 ? 1 : time + step;
	} else {
		time = abs(time) - step < 0 ? 0 : time - sign(time) * step;
	}
	if (controllerIn->doUseController() && leftStickSens) {
		if (!doJump)
			time = abs(time) > abs(controllerIn->getLStickX()) ? sign(time) * abs(controllerIn->getLStickX() * controllerIn->getLStickX()) : time;
	} 
	vel.x = (lerp(0, sideMove, abs(time)) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);

	// JUMP
	if (doJump < 2 && jumpButtonDown) {
		onGround = false;
		vel.y = -g * t_h;
		doJump++;
		if (goLeft && vel.x > 0 || goRight && vel.x < 0) 
			vel.x = (lerp(0, sideMove, abs(time = -0.5 * sign(time))) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);
		else if (!goLeft && !goRight)
			vel.x = (lerp(0, sideMove, abs(time /= 5)) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);
	}

	moveBy(vel * delta + 0.5f * acc * delta * delta);

	vel += (!jumpButtonPressed || vel.y < 0 ? fastFall : 1) * acc * delta;
}

void Player::updateActionBuffer() {
	for (auto &i : actionBuffer) {
		float time = 0;
		int count = 0;
		i.second.down = false;
		i.second.up = false;
		i.second.pressed = false;
		for (auto j : actionMapping[i.first].kButtons) {
			i.second.down = i.second.down || keyIn->isKeyDown(j);
			i.second.up = i.second.up || keyIn->isKeyUp(j);
			i.second.pressed = i.second.pressed || keyIn->isKeyPressed(j);
		}
		for (auto j : actionMapping[i.first].mButtons) {
			i.second.down = i.second.down || mouseIn->isButtonDown(j);
			i.second.up = i.second.up || mouseIn->isButtonUp(j);
			i.second.pressed = i.second.pressed || mouseIn->isButtonPressed(j);
		}
		for (auto j : actionMapping[i.first].cButtons) {
			if ((int) j < (int) ControllerButton::AXIS_START) {
				i.second.down = i.second.down || controllerIn->isButtonDown(j);
				i.second.up = i.second.up || controllerIn->isButtonUp(j);
				i.second.pressed = i.second.pressed || controllerIn->isButtonPressed(j);
			} else {
				i.second.down = i.second.down || controllerIn->isAxisDown(j);
				i.second.up = i.second.up || controllerIn->isAxisUp(j);
				i.second.pressed = i.second.pressed || controllerIn->isAxisPressed(j);
			}
		}
	}
}