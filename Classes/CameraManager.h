#ifndef CAMERA_MANAGER
#define CAMERA_MANAGER

#include "cocos2d.h"
#include "PerlinNoise.h"

class CameraManager {
private:
	CameraManager() = default;
	static CameraManager* instance;
public:
	static CameraManager* getInstance();

	void update(float delta);

	void moveBy(cocos2d::Vec2 position);

	void lazyFollowTarget(cocos2d::Node* target, float timeToReach = 0.5f);

	cocos2d::Vec2 getPosition() const { return position; }

	void setCamera(cocos2d::Camera* camera);

	void setTrauma(float trauma) { this->trauma = trauma < 0 ? 0 : trauma > 1 ? 1 : trauma; }
	void addTrauma(float trauma) {
		this->trauma = this->trauma + trauma < 0 ? 0 : this->trauma + trauma > 1 ? 1 : this->trauma + trauma;
	}


private:
	cocos2d::Camera* camera;

	static PerlinNoise perlin;

	bool doLazyFollow;
	cocos2d::Node* followTarget;
	float timeToTarget;

	cocos2d::Vec2 position;
	cocos2d::Vec2 offset;
	float angle;
	float trauma;

	float totalTime;

	const cocos2d::Vec2 maxOffset = cocos2d::Vec2(200, 200);
	const float maxAngle = 20;
};



#endif
