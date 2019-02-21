#include "Player.h"

#include "Algorithms.h"

#include "GameSettings.h"

namespace Retry
{

Retry::Player::Player(std::string path, cocos2d::Vec2 pos)
{
	init(path, pos);

	addButtonToMapping("jump", KeyCode::SPACE);
	addButtonToMapping("jump", ControllerButton::A);

	addButtonToMapping("left", KeyCode::A);
	addButtonToMapping("left", ControllerButton::LEFT_STICK_LEFT);

	addButtonToMapping("right", KeyCode::D);
	addButtonToMapping("right", ControllerButton::LEFT_STICK_RIGHT);

	addButtonToMapping("leftStick", ControllerButton::LEFT_STICK_X);
}

float lerp(float p0, float p1, float t)
{
	t = clamp(t, 0, 1);
	return p1 * t + p0 * (1 - t);
}

int sign(float x)
{
	return x < 0 ? -1 : x > 0 ? 1 : 0;
}

void Retry::Player::update(float delta)
{
	updateActionBuffer();

	bool jumpButtonDown = isActionDown("jump");
	bool jumpButtonPressed = isActionPressed("jump");

	bool goLeft = isActionPressed("left");
	bool goRight = isActionPressed("right");

	// Side Movement Constants and Variables
	static const float animMoveTime = 0.12f * 450.f;
	static const float sideMove = 700;
	static const float timeToMax = 0.1f;

	// Jumping Constants and Variables
	static const float h = 400;
	static const float t_h = 0.65f;
	static const float g = (-2 * h) / (t_h * t_h);
	static const float fastFall = 2.f;
	static bool hasMoved = false;

	acceleration = cocos2d::Vec2(0, g);

	// LEFT AND RIGHT MOVEMENT
	if (goLeft != goRight)
	{
		acceleration.x = (goLeft ? -1 : 1) * sideMove / timeToMax / (!onGround ? (velocity.y > 100 ? 5.0f : 3.0f) : 1);
	} else
	{
		acceleration.x = !onGround ? 0 : -sign(velocity.x) * sideMove / timeToMax;
		if (sign(velocity.x) * (velocity.x + acceleration.x * delta) < 0)
			acceleration.x = velocity.x = 0;
	}
	velocity.x += acceleration.x * delta;
	velocity.x = sign(velocity.x) * clamp(abs(velocity.x), 0, sideMove + (doJump ? 100 : 0));
	// !LEFT AND RIGHT MOVEMENT

	// JUMP
	if (onGround || Retry::Config::doDebug()) doJump = 0;
	if (doJump < 2 && jumpButtonDown)
	{
		onGround = false;
		velocity.y = -g * t_h;
		doJump++;

		if (goLeft && velocity.x > 0 || goRight && velocity.x < 0)
		{
			velocity.x *= -0.5f;
		} else if (!goLeft && !goRight)
		{
			velocity.x *= 0.2f;
		}
	}
	// !JUMP


	// ANIMATION HANDLING
	float multi = Controller::isAxisPressed(ControllerButton::RIGHT_TRIGGER) ? 4 : 1;
	if (velocity.x != 0) sprite->setFlippedX(velocity.x < 0);
	if (onGround)
	{
		if (velocity.x != 0)
		{
			runAnimation("run", animMoveTime / sideMove * multi);
		} else
		{
			runAnimation("idle", 0.12f * multi);
		}
	} else
	{
		runAnimation("jump", 0.1f * multi);
	}
	// !ANIMATION HANDLING

	moveBy(velocity * delta + 0.5f * acceleration * delta * delta);

	velocity.y += (!jumpButtonPressed || velocity.y < 0 ? fastFall : 1) * acceleration.y * delta;
	velocity.y = clamp(velocity.y, -2000, 2000);
}

void Retry::Player::updateActionBuffer()
{
	for (auto &i : actionBuffer)
	{
		float time = 0;
		int count = 0;
		i.second.down = false;
		i.second.up = false;
		i.second.value = 0;
		for (auto j : actionMapping[i.first].kButtons)
		{
			i.second.down = i.second.down || Retry::Keyboard::isKeyDown(j);
			i.second.up = i.second.up || Retry::Keyboard::isKeyUp(j);
			i.second.value = i.second.value || Retry::Keyboard::isKeyPressed(j);
		}
		for (auto j : actionMapping[i.first].mButtons)
		{
			i.second.down = i.second.down || Retry::Mouse::isButtonDown(j);
			i.second.up = i.second.up || Retry::Mouse::isButtonUp(j);
			i.second.value = i.second.value || Retry::Mouse::isButtonPressed(j);
		}
		for (auto j : actionMapping[i.first].cButtons)
		{
			if ((int) j < 16)
			{
				i.second.down = i.second.down || Retry::Controller::isButtonDown(j);
				i.second.up = i.second.up || Retry::Controller::isButtonUp(j);
				i.second.value = i.second.value || Retry::Controller::isButtonPressed(j);
			} else
			{
				i.second.down = i.second.down || Retry::Controller::isAxisDown(j);
				i.second.up = i.second.up || Retry::Controller::isAxisUp(j);
				float tVal = Retry::Controller::getAxis(j);
				i.second.value = tVal != 0 && Retry::Controller::isAxisPressed(j) ? tVal : i.second.value;
			}
		}
	}
}

void Player::addButtonToMapping(const std::string &action, const Retry::KeyCode &button)
{
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].kButtons.push_back(button);
}

void Player::addButtonToMapping(const std::string &action, const Retry::MouseButton &button)
{
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].mButtons.push_back(button);
}

void Player::addButtonToMapping(const std::string &action, const Retry::ControllerButton &button)
{
	if (actionBuffer.find(action) == actionBuffer.end())
		actionBuffer[action];
	actionMapping[action].cButtons.push_back(button);
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::KeyCode &button)
{
	auto keyMap = &actionMapping[action].kButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++)
	{
		if (keyMap->at(i) == button)
		{
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::MouseButton &button)
{
	auto keyMap = &actionMapping[action].mButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++)
	{
		if (keyMap->at(i) == button)
		{
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}

void Player::removeButtonFromMapping(const std::string &action, const Retry::ControllerButton &button)
{
	auto keyMap = &actionMapping[action].cButtons;
	for (int i = 0, n = keyMap->size(); i < n; i++)
	{
		if (keyMap->at(i) == button)
		{
			keyMap->erase(keyMap->begin() + i);
			return;
		}
	}
}


}