#include "CameraManager.h"

CameraManager* CameraManager::instance = 0;

PerlinNoise CameraManager::perlin = PerlinNoise(128);

CameraManager* CameraManager::getInstance() {
	return instance == 0 ? instance = new CameraManager() : instance;
}

void CameraManager::update(float delta) {
	static PerlinNoise p1 = PerlinNoise(126), p2 = PerlinNoise(127), p3 = PerlinNoise(128);

	angle = maxAngle * trauma * trauma * (p1.noise(totalTime, totalTime, 0) * 2 - 1);
	offset.x = maxOffset.x * trauma * trauma * (p2.noise(totalTime, totalTime, 0) * 2 - 1);
	offset.y = maxOffset.y * trauma * trauma * (p3.noise(totalTime, totalTime, 0) * 2 - 1);

	if (doLazyFollow) {
		moveBy((followTarget->getPosition() - position) * delta / timeToTarget);
	}

	camera->setPosition(position.x + offset.x, position.y + offset.y);
	camera->setRotation(angle);

	addTrauma(-1 * delta);
	totalTime += delta * 10;
}

void CameraManager::moveBy(cocos2d::Vec2 position) {
	this->position += position;
}

void CameraManager::lazyFollowTarget(cocos2d::Node* target, float timeToReach) {
	//followTarget = target;
	//if (followTarget == nullptr) doLazyFollow = false;
	//else {
	//	timeToTarget = timeToReach;
	//	doLazyFollow = true;
	//}
	doLazyFollow = (bool) (followTarget = target);
	timeToTarget = (timeToReach + 0.01f + abs(timeToReach - 0.01f)) / 2;
}

void CameraManager::setCamera(cocos2d::Camera * camera) {
	this->camera = camera;
	this->position = camera->getPosition();
	this->angle = camera->getRotation();
}

