#include "Player.h"

Player::Player(std::string path, Vec2 pos) {
	load(path, pos);
}

void Player::update(float delta) {
	using namespace Retry;
	static auto space = KeyCode::SPACE;
	float jumpFallMultiplier = 7.0f;
	float fastFallMultiplier = 2.0f;

	const char nJumpFrames = 7;

	static char doJump = 0;
	static char jumpTimer = 0;
	float move = 800;
	static Vec2 accel = Vec2(0, -2000);
	static Vec2 veloc = Vec2(0, 0);

	if (keyIn->isKeyPressed(KeyCode::A)) {
		veloc.x = -move;
	}
	if (keyIn->isKeyPressed(KeyCode::D)) {
		veloc.x = move;
	}

	if (doJump < 2 && keyIn->isKeyDown(KeyCode::SPACE)) {
		veloc.y = 2000;
		doJump++;
		jumpTimer = 0;
	}

	if (jumpTimer <= nJumpFrames && keyIn->isKeyUp(space)) jumpTimer = nJumpFrames + 1;
	if (!keyIn->isKeyPressed(space) || jumpTimer >= nJumpFrames) {
		if (veloc.y > 0) {
			veloc += Vec2(0, 1) * accel.y * jumpFallMultiplier * delta;
		} else if (jumpTimer >= nJumpFrames * 2.5f) {
			veloc += Vec2(0, 1) * accel.y * fastFallMultiplier * delta;
		} else {
			veloc.y = 0;
		}
	}


	if (sprite->getPosition().y < 100) {
		sprite->setPositionY(100);
		doJump = 0;
	}
	//veloc += Vec2(0, 1) * accel.y * delta;
	if (veloc.y < -1100) veloc.y = -1100;
	moveBy(veloc * delta);
	jumpTimer++;
	veloc.x = 0;
}
