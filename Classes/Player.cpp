#include "Player.h"

#include "CameraManager.h"

#include "Algorithms.h"

//Retry::MouseManager* Player::mouseIn = Retry::MouseManager::getInstance();
//Retry::ControllerManager* Player::controllerIn = Retry::ControllerManager::getInstance();

namespace Retry
{

std::unordered_map<std::string, KeyMap> Retry::Player::actionMapping;

Retry::Player::Player(std::string path, cocos2d::Vec2 pos)
{
	init(path, pos);

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

float lerp(float p0, float p1, float t)
{
	t = clamp(t, 0, 1);
	return p1 * t + p0 * (1 - t);
}

float lerpSq(float p0, float p1, float t)
{
	t = clamp(t, 0, 1);
	return p1 * t * t + p0 * (1 - t * t);
}

int sign(float x)
{
	return x < 0 ? -1 : x > 0 ? 1 : 0;
}

void Retry::Player::update(float delta)
{
	updateActionBuffer();

	using namespace Retry;

	bool jumpButtonDown = isActionDown("jump");
	bool jumpButtonPressed = isActionPressed("jump");

	bool goLeft = isActionPressed("left"),
		goRight = isActionPressed("right");

	bool leftStickSens = Retry::Controller::isAxisPressed(ControllerButton::LEFT_STICK_LEFT) ||
		Retry::Controller::isAxisPressed(ControllerButton::LEFT_STICK_RIGHT);

// Side Movement Constants and Variables
	static const float animMoveTime = 0.12f * 450.f;
	static const float sideMove = 700;
	static const float timeToMax = 0.1f;
	static float time = 0;

	// Jumping Constants and Variables
	static const float h = 400;
	static const float t_h = 0.5f;
	static const float g = (-2 * h) / (t_h * t_h);
	static const float fastFall = 2.f;
	static int doJump = 0;
	static bool hasMoved = false;
	static bool onGround = true;

	acceleration = cocos2d::Vec2(0, g);

	// LANDING ON THE GROUND
	static const float groundHeight = 50;
	if (position.y < groundHeight)
	{
		if (!onGround)
			Retry::Camera::addTrauma(0.3f);

		position.y = groundHeight;
		doJump = 0;
		hasMoved = false;
		onGround = true;
	}

	float step = (!doJump || goLeft || goRight) * (delta / timeToMax) / (doJump ? (velocity.y > 100 ? 5.0f : 3.0f) : 1);

	if (goLeft || goRight) hasMoved = true;

	// MOVEMENT
	if (goLeft && !goRight)
	{
		time = time - step < -1 ? -1 : time - step;
	} else if (goRight && !goLeft)
	{
		time = time + step > 1 ? 1 : time + step;
	} else
	{
		time = abs(time) - step < 0 ? 0 : time - sign(time) * step;
	}
	if (Retry::Controller::doUseController() && leftStickSens)
	{
		if (!doJump)
			time = abs(time) > abs(Retry::Controller::getLStickX()) ? sign(time) * abs(Retry::Controller::getLStickX() * Retry::Controller::getLStickX()) : time;
	}
	velocity.x = (lerp(0, sideMove, abs(time)) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);

	// JUMP
	if (doJump < 2 && jumpButtonDown)
	{
		onGround = false;
		velocity.y = -g * t_h;
		doJump++;
		if (goLeft && velocity.x > 0 || goRight && velocity.x < 0)
			velocity.x = (lerp(0, sideMove, abs(time = -0.5 * sign(time))) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);
		else if (!goLeft && !goRight)
			velocity.x = (lerp(0, sideMove, abs(time /= 5)) + (doJump ? lerp(0, 100, abs(time)) : 0)) * sign(time);
	}

	auto v = velocity * delta + 0.5f * acceleration * delta * delta;
	
	// Animation Stuff
	if (time != 0) sprite->setFlippedX(time < 0);
	if (onGround)
	{
		if (time != 0)
		{
			runAnimation("run", animMoveTime / sideMove);
		} else
		{
			runAnimation("idle", 0.15f);
		}
	} else
	{
		runAnimation("jump", 0.05f);
	}
	moveBy(v);
	if (onGround) position.y = groundHeight;

	velocity += (!jumpButtonPressed || velocity.y < 0 ? fastFall : 1) * acceleration * delta;
	setPosition(position);
}

void Retry::Player::updateActionBuffer()
{
	for (auto &i : actionBuffer)
	{
		float time = 0;
		int count = 0;
		i.second.down = false;
		i.second.up = false;
		i.second.pressed = false;
		for (auto j : actionMapping[i.first].kButtons)
		{
			i.second.down = i.second.down || Retry::Keyboard::isKeyDown(j);
			i.second.up = i.second.up || Retry::Keyboard::isKeyUp(j);
			i.second.pressed = i.second.pressed || Retry::Keyboard::isKeyPressed(j);
		}
		for (auto j : actionMapping[i.first].mButtons)
		{
			i.second.down = i.second.down || Retry::Mouse::isButtonDown(j);
			i.second.up = i.second.up || Retry::Mouse::isButtonUp(j);
			i.second.pressed = i.second.pressed || Retry::Mouse::isButtonPressed(j);
		}
		for (auto j : actionMapping[i.first].cButtons)
		{
			if ((int) j < (int) ControllerButton::AXIS_START)
			{
				i.second.down = i.second.down || Retry::Controller::isButtonDown(j);
				i.second.up = i.second.up || Retry::Controller::isButtonUp(j);
				i.second.pressed = i.second.pressed || Retry::Controller::isButtonPressed(j);
			} else
			{
				i.second.down = i.second.down || Retry::Controller::isAxisDown(j);
				i.second.up = i.second.up || Retry::Controller::isAxisUp(j);
				i.second.pressed = i.second.pressed || Retry::Controller::isAxisPressed(j);
			}
		}
	}
}
}