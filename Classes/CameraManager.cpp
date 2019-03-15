#include "CameraManager.h"

#include "GameSettings.h"

#include "KeyboardManager.h"

namespace Retry {

PerlinNoise Camera::perlin = PerlinNoise(128);
cocos2d::Camera* Camera::camera;

char Camera::targetingMask;
cocos2d::Node* Camera::followTarget;
std::vector<std::pair<cocos2d::Node*, cocos2d::Vec2>> Camera::focusTargets;
float Camera::timeToTarget;

float Camera::minX = 0;
float Camera::minY = 0;
float Camera::maxX = 1000000;
float Camera::maxY = 1000000;

cocos2d::Vec2 Camera::position;
cocos2d::Vec2 Camera::offset;
float Camera::angle;
float Camera::trauma;

float Camera::totalTime;

const cocos2d::Vec2 Camera::maxOffset = cocos2d::Vec2(200, 200);
const float Camera::maxAngle = 20;

float normalizeInRange(float x, float a, float b) {
	;
	   //float y = (x - a) / (b - a);
	return x <= a ? 0 : x >= b ? 1 : (x - a) / (b - a);
}

void Camera::update(float delta) {
	static PerlinNoise p1 = PerlinNoise(126), p2 = PerlinNoise(127), p3 = PerlinNoise(128);

	if (Config::doScreenShake()) {
		angle = maxAngle * trauma * trauma * (p1.noise(totalTime, totalTime, 0) * 2 - 1);
		offset.x = maxOffset.x * trauma * trauma * (p2.noise(totalTime, totalTime, 0) * 2 - 1);
		offset.y = maxOffset.y * trauma * trauma * (p3.noise(totalTime, totalTime, 0) * 2 - 1);
	}

	cocos2d::Vec2 targetPoint = cocos2d::Vec2::ZERO;

	if (targetingMask & 0x2) {
		float targetOffsetX = (cocos2d::Vec2(0.5f, 0.5f) - followTarget->getAnchorPoint()).x * followTarget->getContentSize().width;
		float targetOffsetY = (cocos2d::Vec2(0.5f, 0.5f) - followTarget->getAnchorPoint()).y * followTarget->getContentSize().height;

		//cocos2d::Vec2 targetPoint = followTarget->getPosition() + cocos2d::Vec2(targetOffsetX, targetOffsetY);
		//if (targetPoint.x < cocos2d::Director::getInstance()->getVisibleSize().width / camera->getParent()->getScale() / 2)
		//	targetPoint.x = cocos2d::Director::getInstance()->getVisibleSize().width / camera->getParent()->getScale() / 2;
		//if (targetPoint.y < cocos2d::Director::getInstance()->getVisibleSize().height / camera->getParent()->getScale() / 2)
		//	targetPoint.y = cocos2d::Director::getInstance()->getVisibleSize().height / camera->getParent()->getScale() / 2;

	} else if (targetingMask & 0x4) {
		float totalInfluence = 1;
		cocos2d::Vec2 posToAvg = followTarget->getPosition();
		cocos2d::Vec2 minMaxX = cocos2d::Vec2(followTarget->getPositionX(), followTarget->getPositionX());
		cocos2d::Vec2 minMaxY = cocos2d::Vec2(followTarget->getPositionY(), followTarget->getPositionY());

		for (auto i : focusTargets) {
			if (i.first == followTarget) continue;
			cocos2d::Vec2 lmao = followTarget->getPosition() - i.first->getPosition();
			lmao.y *= 1920.0f / 1080.0f;
			float influence = 1 - normalizeInRange(lmao.length(), i.second.x, i.second.y);

			if (influence != 0) {
				if (i.first->getPositionX() < minMaxX.x)
					minMaxX.x = i.first->getPositionX();
				if (i.first->getPositionX() > minMaxX.y)
					minMaxX.y = i.first->getPositionX();

				if (i.first->getPositionY() < minMaxY.x)
					minMaxY.x = i.first->getPositionY();
				if (i.first->getPositionY() > minMaxY.y)
					minMaxY.y = i.first->getPositionY();

				posToAvg += i.first->getPosition() * influence;
				totalInfluence += influence;
			}
		}
		//float xx = (minMaxX.x + minMaxX.y) * 0.5f, yy = (posToAvg / totalInfluence).y;
		//moveBy((posToAvg / totalInfluence - position) * delta / timeToTarget);

		targetPoint = posToAvg / totalInfluence;

		float newScale = clamp(MIN(1920 / (1.5f * (minMaxX.y - minMaxX.x)), 1080 / (1.5f * (minMaxY.y - minMaxY.x))), 1.1f, 1.6f);
		if (totalInfluence <= 1) newScale = 1.1f;

		camera->getParent()->setScale(camera->getParent()->getScale() + (newScale - camera->getParent()->getScale()) * delta / 0.5f);
		//if (camera->getParent()->getScale() > 1) camera->getParent()->setScale(1);
	}

	float wid = cocos2d::Director::getInstance()->getVisibleSize().width / camera->getParent()->getScale() / 2;
	float hei = cocos2d::Director::getInstance()->getVisibleSize().height / camera->getParent()->getScale() / 2;
	if (targetPoint.x - wid < minX)
		targetPoint.x = minX + wid;
	if (targetPoint.y - hei < minY)
		targetPoint.y = minY + hei;
	if (targetPoint.x + wid > maxX)
		targetPoint.x = maxX - wid;
	if (targetPoint.y + hei > maxY)
		targetPoint.y = maxY - hei;

	moveBy((targetPoint - position) * delta / timeToTarget);

	camera->setPosition(position * camera->getParent()->getScale() + offset);

	camera->setRotation(angle);

	addTrauma(-1 * delta);
	totalTime += delta * 10;
}

void Camera::moveBy(cocos2d::Vec2 position) {
	Camera::position += position;
}

void Camera::lazyFollowTarget(cocos2d::Node* target, float timeToReach) {
	targetingMask = int(!!(followTarget = target)) << 1;
	timeToTarget = (timeToReach + 0.01f + abs(timeToReach - 0.01f)) / 2;
}

void Camera::addTarget(cocos2d::Node * target, float nearThreshHold, float farThreshHold) {
	for (auto i : focusTargets) if (i.first == target) return;

	targetingMask = 0x4;
	focusTargets.push_back(std::make_pair(target, cocos2d::Vec2(nearThreshHold, farThreshHold)));
}

void Camera::setTimeToTarget(float f) {
	timeToTarget = f;
}

void Camera::setCamera(cocos2d::Camera * camera) {
	Camera::camera = camera;
	Camera::position = camera->getPosition();
	Camera::angle = camera->getRotation();

	camera->getParent()->setAnchorPoint(cocos2d::Vec2(0, 0));

	followTarget = nullptr;
	focusTargets.clear();

	minX = 0;
	minY = 0;
	maxX = 100000;
	maxY = 100000;
}

void Camera::transformUI(cocos2d::Node* ui) {
	ui->setPosition((Camera::position + Camera::offset));
	ui->setRotation(Camera::angle);
	ui->setScale(1 / camera->getParent()->getScale());
}

}