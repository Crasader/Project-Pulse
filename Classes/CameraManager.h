#ifndef CAMERA_MANAGER
#define CAMERA_MANAGER

#include "cocos2d.h"
#include "PerlinNoise.h"

#include "Algorithms.h"

using Retry::clamp;

namespace Retry {

class Camera abstract {
public:
	static void update(float delta);

	static void moveBy(cocos2d::Vec2 position);

	static void lazyFollowTarget(cocos2d::Node* target, float timeToReach = 0.5f);
	static void addTarget(cocos2d::Node* target, float nearThreshHold = 1920 * 1 / 4, float farThreshHold = 1920 * 4 / 5);

	static void setTimeToTarget(float f);

	static void doLazyFollow() { Camera::targetingMask = 0x1; }
	static void doFocusFollow() { Camera::targetingMask = 0x2; }
	static void doMultiFollow() { Camera::targetingMask = 0x4; }

	static cocos2d::Vec2 getPosition() { return Camera::position; }
	static void setPosition(cocos2d::Vec2 pos) { Camera::position = pos; }

	static cocos2d::Camera* getCamera() { return Camera::camera; }
	static void setCamera(cocos2d::Camera* camera);

	static void setTrauma(float trauma) { Camera::trauma = clamp(trauma, 0, 1); }

	static void addTrauma(float trauma) { Camera::trauma = clamp(Camera::trauma + trauma, 0, 1); }

	static void transformUI(cocos2d::Node* ui);

	static float getMinX() { return minX; }
	static float getMinY() { return minY; }
	static float getMaxX() { return maxX; }
	static float getMaxY() { return maxY; }

	static void setMinX(float x) { Camera::minX = x; }
	static void setMinY(float y) { Camera::minY = y; }
	static void setMaxX(float x) { Camera::maxX = x; }
	static void setMaxY(float y) { Camera::maxY = y; }

private:
	static cocos2d::Camera* camera;

	static PerlinNoise perlin;

	static char targetingMask;
	static cocos2d::Node* followTarget;
	static std::vector<std::pair<cocos2d::Node*, cocos2d::Vec2>> focusTargets;
	static float timeToTarget;

	static float minX;
	static float minY;
	static float maxX;
	static float maxY;

	static cocos2d::Vec2 position;
	static cocos2d::Vec2 offset;
	static float angle;
	static float trauma;

	static float totalTime;

	static const cocos2d::Vec2 maxOffset;
	static const float maxAngle;
};
}


#endif
