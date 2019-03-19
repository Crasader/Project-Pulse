/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "MenuScene.h"
#include "OptionsMenu.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

#include "ui/CocosGUI.h"

#include "CollisionBody.h"

#include "Level.h"

#include "GameSettings.h"

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Rect;

using Retry::Keyboard;
using Retry::KeyCode;
using Retry::Mouse;
using Retry::MouseButton;
using Retry::Controller;
using Retry::ControllerButton;

cocos2d::Scene* MainScene::createScene() {
	auto scene = MainScene::create();

	return scene;
}

void MainScene::onExit() {
	Scene::onExit();
}

void MainScene::onEnter() {
	Scene::onEnter();
}

bool MainScene::init() {
	if (!Scene::init()) {
		return false;
	}

	gui = cocos2d::Node::create();

	initPlayer(Vec2(150, 400));

	//pulseParticle = new Retry::Entity("cybercoppulse.png", Vec2(0, 0));
	//pulseParticle->initAnimation("pulse", "cybercoppulse.png", Vec2(0, 0), Vec2(64, 64), 22);
	//pulseParticle->runAnimation("pulse", 0.1f);
	//pulseParticle->getSprite()->setPosition(Vec2(32,32));
	//player->getSprite()->addChild(pulseParticle->getSprite());

	enemy = new Retry::GoonEnemy(Vec2(6550, 2000));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::TutorialEnemy(Vec2(4400, 2000));
	enemy->getSprite()->setScale(2);
	enemy->setFlippedX(true);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::GoonEnemy(Vec2(10368, 1800));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::GoonEnemy(Vec2(11328, 2000));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::GoonEnemy(Vec2(13760, 7800));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::GoonEnemy(Vec2(13184, 8000));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	enemy = new Retry::GoonEnemy(Vec2(12608, 7800));
	enemy->getSprite()->setScale(2);
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	healthBarBack = cocos2d::Sprite::create("healthbar.png", Rect(0, 0, 128, 32));
	healthBarBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	healthBarBack->setPosition(Vec2(50, 50));
	healthBarBack->setScale(3);

	healthBarFront = cocos2d::Sprite::create("healthbar.png", Rect(0, 32, 128, 32));
	healthBarFront->setAnchorPoint(healthBarBack->getAnchorPoint());
	healthBarFront->setPosition(healthBarBack->getPosition());
	healthBarFront->setScale(healthBarBack->getScale());

	gui->addChild(healthBarBack, 100);
	gui->addChild(healthBarFront, healthBarBack->getLocalZOrder() + 1);

	// ADD ALL GUI AND ACTORS BEFORE HERE
	this->scheduleUpdate();

	for (auto i : gui->getChildren())
		i->setPosition(i->getPosition() - cocos2d::Director::getInstance()->getVisibleSize() / 2);

	this->addChild(gui, 100);

	Retry::Camera::setCamera(this->getDefaultCamera());

	Retry::Camera::setPosition(player->getSprite()->getPosition());

	level = new Retry::Level(1);
	this->addChild(level->getLevelDraw());

	Retry::Camera::lazyFollowTarget(player->getSprite(), 0.25f);

	for (auto i : actorList) {
		Retry::Camera::addTarget(i->getSprite());
	}

	Retry::Camera::setPosition(player->getPosition());

	Keyboard::createListener(_eventDispatcher, this);
	Mouse::createListener(_eventDispatcher, this);
	Controller::createListener(this);

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender) {
	cocos2d::Director::getInstance()->end();
}

void MainScene::update(float delta) {

	if (Keyboard::isKeyDown(KeyCode::ESCAPE) || Controller::isButtonDown(ControllerButton::START))
		cocos2d::Director::getInstance()->pushScene(OptionsMenu::createScene());

	if (Keyboard::isKeyDown(KeyCode::Q) || Controller::isButtonDown(ControllerButton::BACK))
		cocos2d::Director::getInstance()->replaceScene(MenuScene::create());

	//if (Keyboard::isKeyDown(KeyCode::F2) || Controller::isButtonDown(ControllerButton::LEFT_BUMPER))
	//	toggleDebug();

	Retry::Config::setDebug(Controller::isAxisPressed(ControllerButton::LEFT_TRIGGER) || Keyboard::isKeyPressed(KeyCode::CAPS_LOCK));

	player->update(delta);
	for (auto i : actorList) {
		i->update(delta);

		i->doTerrainCollision(level, delta);

		player->doAttackOnActor(i);
		i->doAttackOnActor(player);
	}

	static float orgWidth = healthBarFront->getTextureRect().size.width;
	float newWidth = player->getHealthRatio() * orgWidth;
	healthBarFront->setTextureRect(Rect(Vec2(0, 32), Size(newWidth, healthBarFront->getTextureRect().size.height)));

	Retry::Camera::update(delta);

	static bool doFull = false;
	if (Keyboard::isKeyDown(KeyCode::F11)) {
		if (!(doFull = !doFull))
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setWindowed(1920, 1080);
		else
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
	}

	Retry::Camera::transformUI(gui);
}

void MainScene::initPlayer(cocos2d::Vec2 position) {
	player = new Retry::Player(position);

	this->addChild(player->getSprite(), 100);
}

void MainScene::toggleDebug() {
	//Retry::Config::toggleDebug();
	static bool doDraw = false;
	doDraw = !doDraw;
	for (auto i : actorList) {
		i->getHitBox()->setDebugDraw(doDraw);
	}
}

void MainScene::doAICalculations() {
	//auto playerBBox = Retry::Collision::worldSpaceRect(player->getSprite(), player->getHurtBox()->getBoundingBox());
	cocos2d::Rect playerBBox = player->getHitBox()->getWorldBoundingBox();

	for (auto i : actorList) {
		if (i == player) continue;
		cocos2d::Rect actorBBox = i->getHitBox()->getWorldBoundingBox();

		//if ((player->getPosition() - i->getPosition()).getLengthSq() < 1500 * 1500) {
		//	if (player->getPosition().x > i->getPosition().x)
		//		player->canMoveOn = false;
		//	if (!(i->doPracticeDummy && i->getHealth() == i->getMaxHealth()) && abs(playerBBox.origin.x - actorBBox.origin.x + (playerBBox.size.width - actorBBox.size.width) * 0.5f) > 180) {
		//		if (playerBBox.origin.x - actorBBox.origin.x < 0)
		//			i->bufferAction("left");
		//		else
		//			i->bufferAction("right");

		//		if (i->getVelocity().x == 0)
		//			i->bufferAction("jump");
		//	} else {
		//		static bool doButton = true;
		//		if ((doButton && i->getCurrentAttackKey() & 0b1111) || rand() % 1000 < 7) i->bufferAction("punch");
		//		doButton ^= 1;
		//	}

		//}
	}
	for (int i = 1; i < actorList.size(); i++) {
		if (actorList[i]->getHealth() <= 0) {
			actorList[i]->setPosition(Vec2(-1000, -1000));
			//delete actorList[i];
			actorList.erase(actorList.begin() + i--);
		}
	}
}