#pragma once
#include "Actor.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ControllerManager.h"
#include "AudioPlayer.h"

namespace {

struct KeyMap {
	std::vector<Retry::KeyCode> kButtons;
	std::vector<Retry::MouseButton> mButtons;
	std::vector<Retry::ControllerButton> cButtons;
};
}

namespace Retry
{

enum PulseMode : char {
	REST,
	PULSE,
	COOLDOWN
};

class Player : public Actor
{
public:
	Player(std::string path, cocos2d::Vec2 pos);

	void update(const float& delta) override;

	void updateAnimations(const float& delta) override;

	virtual bool doTerrainCollision(Level* level, const float &delta) override;

	void addButtonToMapping(const std::string &action, const Retry::KeyCode &button);
	void addButtonToMapping(const std::string &action, const Retry::MouseButton &button);
	void addButtonToMapping(const std::string &action, const Retry::ControllerButton &button);

	void removeButtonFromMapping(const std::string &action, const Retry::KeyCode &button);
	void removeButtonFromMapping(const std::string &action, const Retry::MouseButton &button);
	void removeButtonFromMapping(const std::string &action, const Retry::ControllerButton &button);

	virtual void doAttackOnActor(Actor* actor) override;
	
	char getMode() { return pulseCooldown > 0 ? COOLDOWN : isPulse ? PULSE : REST; }

	bool canMoveOn = true;

private:
	std::unordered_map<std::string, KeyMap> actionMapping;

	void updateActionBuffer(const float& delta) override;

	void performSideMovement(const float& delta) override;

	void performJump(const float& delta) override;

	void updatePulseMode(const float& delta);

	float doCameraCollision(Retry::Level* level, const cocos2d::Rect &boundingBox);

	bool isPulse = false;

	float pulseCooldown = 0;
	float pulseCooldownTimer = 3.0f;

	float pulseDamageBoost = 1.5f;

	float pulseKnockbackBoost = 1.5f;
};

}