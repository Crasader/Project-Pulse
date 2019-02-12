#ifndef ENTITY
#define ENTITY

#include <string>
#include "cocos2d.h"

#include "CollisionBody.h"

namespace Retry
{

class Entity
{
public:

	Entity() = default;
	Entity(std::string, cocos2d::Vec2);
	~Entity() {}

	void init(std::string path, cocos2d::Vec2 position);
	void kill(float delay = 0.0f);

	virtual void update(float dt) {};

	void moveBy(cocos2d::Vec2 movement);

	// Getters and Setters
	cocos2d::Sprite* getSprite() { return sprite; }
	Retry::Collision::Body* getHurtBox() { return &hurtBox; }
	Retry::Collision::Body* getHitBox() { return &hitBox; }

	float getWidth() { return sprite->getBoundingBox().size.width; }
	float getHeight() { return sprite->getBoundingBox().size.height; }

	cocos2d::Vec2 getPosition() { return position; }
	cocos2d::Vec2 getVelocity() { return velocity; }
	cocos2d::Vec2 getAcceleration() { return acceleration; }

	void setPosition(const cocos2d::Vec2 &position)
	{
		lastPosition = this->position;

		sprite->setPosition(position + sprite->getBoundingBox().size / 2); 
		this->position = position;

		deltaPosition = this->position - this->lastPosition;
	}
	void setVelocity(cocos2d::Vec2 velocity) { this->velocity = velocity; }
	void setAcceleration(cocos2d::Vec2 accel) { this->acceleration = accel; }

	void Entity::initAnimation(std::string action, std::string file, cocos2d::Vec2 startCell, cocos2d::Vec2 frameSize, int numFrames);

	void Entity::runAnimation(std::string action, float totalTime);



protected:

	cocos2d::Sprite* sprite;
	cocos2d::Vec2 position;
	cocos2d::Vec2 velocity;
	cocos2d::Vec2 acceleration;

	cocos2d::Vec2 lastPosition;
	cocos2d::Vec2 deltaPosition;

	float totalTime;
	std::string currentAnimation;
	std::unordered_map<std::string, cocos2d::Animation*> animations;

	Retry::Collision::Body hurtBox;
	Retry::Collision::Body hitBox;
};


}


#endif