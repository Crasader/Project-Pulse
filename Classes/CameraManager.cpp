#include "CameraManager.h"

namespace Retry
{

PerlinNoise Camera::perlin = PerlinNoise(128);
cocos2d::Camera* Camera::camera;

char Camera::targetingMask;
cocos2d::Node* Camera::followTarget;
std::vector<std::pair<cocos2d::Node*, cocos2d::Vec2>> Camera::focusTargets;
float Camera::timeToTarget;

cocos2d::Vec2 Camera::position;
cocos2d::Vec2 Camera::offset;
float Camera::angle;
float Camera::trauma;

float Camera::totalTime;

const cocos2d::Vec2 Camera::maxOffset = cocos2d::Vec2(200, 200);
const float Camera::maxAngle = 20;

float normalizeInRange(float x, float a, float b)
{
	float y = (x - a) / (b - a);
	return clamp(y, 0, 1);
}

void Camera::update(float delta)
{
	static PerlinNoise p1 = PerlinNoise(126), p2 = PerlinNoise(127), p3 = PerlinNoise(128);

	angle = maxAngle * trauma * trauma * (p1.noise(totalTime, totalTime, 0) * 2 - 1);
	offset.x = maxOffset.x * trauma * trauma * (p2.noise(totalTime, totalTime, 0) * 2 - 1);
	offset.y = maxOffset.y * trauma * trauma * (p3.noise(totalTime, totalTime, 0) * 2 - 1);

	if (targetingMask & 0x2)
	{
		// MAKE ACTUAL SIZE
		float targetOffsetX = (cocos2d::Vec2(0.5f, 0.5f) - followTarget->getAnchorPoint()).x * followTarget->getContentSize().width;
		float targetOffsetY = (cocos2d::Vec2(0.5f, 0.5f) - followTarget->getAnchorPoint()).y * followTarget->getContentSize().height;
		
		cocos2d::Vec2 amountToMove = followTarget->getPosition() + cocos2d::Vec2(targetOffsetX, targetOffsetY) - position;
		
		if (amountToMove.getLengthSq() < 1 * delta) position += amountToMove;
		else moveBy(amountToMove * delta / timeToTarget);
	} else if (targetingMask & 0x4)
	{
		float totalInfluence = 2.f;
		cocos2d::Vec2 posToAvg = followTarget->getPosition() * 2.f;
		for (auto i : focusTargets)
		{
			float influence = 1 - normalizeInRange((followTarget->getPosition() - i.first->getPosition()).length(), i.second.x, i.second.y);
			posToAvg += i.first->getPosition() * influence;
			totalInfluence += influence;
		}
		moveBy((posToAvg / totalInfluence - position) * delta / timeToTarget);


		//float totalInfluence = 1;
		//cocos2d::Vec2 posToAvg = followTarget->getPosition() * totalInfluence;
		//cocos2d::Vec2 minMaxX = cocos2d::Vec2(followTarget->getPositionX(), followTarget->getPositionX());
		////cocos2d::Vec2 minMaxY = followTarget->getPosition();

		//for (auto i : focusTargets) {
		//	float influence = 1 - normalizeInRange((followTarget->getPosition() - i.first->getPosition()).length(), i.second.x, i.second.x);
		//	
		//	if (i.first->getPositionX() < minMaxX.x)
		//		minMaxX.x = i.first->getPositionX();
		//	if (i.first->getPositionX() > minMaxX.y)
		//		minMaxX.y = i.first->getPositionX();

		//	//float influence = 1;
		//	posToAvg += i.first->getPosition();
		//	totalInfluence += 1;
		//}
		//float xx = (minMaxX.x + minMaxX.y) * 0.5f, yy = (posToAvg / totalInfluence).y;
		//moveBy((cocos2d::Vec2(xx, yy) - position) * delta / timeToTarget);
		//camera->getParent()->setScale(1280 / (1.5 * (minMaxX.y - minMaxX.x)));
		//if (camera->getParent()->getScale() > 1) camera->getParent()->setScale(1);
	}

	camera->setPosition(cocos2d::Vec2(position.x + offset.x, position.y + offset.y) * camera->getParent()->getScale());
	camera->setRotation(angle);

	addTrauma(-1 * delta);
	totalTime += delta * 10;
}

void Camera::moveBy(cocos2d::Vec2 position)
{
	Camera::position += position;
}

void Camera::lazyFollowTarget(cocos2d::Node* target, float timeToReach)
{
	targetingMask = int(!!(followTarget = target)) << 1;
	timeToTarget = (timeToReach + 0.01f + abs(timeToReach - 0.01f)) / 2;
}

void Camera::addTarget(cocos2d::Node * target, float nearThreshHold, float farThreshHold)
{
	for (auto i : focusTargets) if (i.first == target) return;

	targetingMask = 0b100;
	focusTargets.push_back(std::make_pair(target, cocos2d::Vec2(nearThreshHold, farThreshHold)));
}

void Camera::setTimeToTarget(float f)
{
	timeToTarget = f;
}

void Camera::setCamera(cocos2d::Camera * camera)
{
	Camera::camera = camera;
	Camera::position = camera->getPosition();
	Camera::angle = camera->getRotation();

	camera->getParent()->setAnchorPoint(cocos2d::Vec2(0, 0));
}

void Camera::transformUI(cocos2d::Node* ui)
{
	ui->setPosition((Camera::position + Camera::offset));
	ui->setRotation(Camera::angle);
	ui->setScale(1 / camera->getParent()->getScale());
}

}