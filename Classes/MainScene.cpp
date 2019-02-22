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

	enemy = new Retry::Actor("HelloWorld.png", Vec2(6000, 1000));
	enemy->getSprite()->setScale(0.5f);
	enemy->getHurtBox()->addRect(Vec2(0, 0), Size(200, 250));
	this->addChild(enemy->getSprite());

	actorList.emplace_back(enemy);

	auto cobble = cocos2d::Sprite::create("cobblestone.png");
	auto cSize = cobble->getContentSize();
	auto sSize = cocos2d::Director::getInstance()->getVisibleSize();


	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 3; i++) {
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 3; j++) {
			auto newCobble = cocos2d::Sprite::create("cobblestone.png");
			newCobble->setAnchorPoint(Vec2(0, 0));
			newCobble->setPosition(i * cSize.width, j * cSize.height);
			newCobble->setColor(cocos2d::Color3B(127, 127, 127));
			this->addChild(newCobble, -100);
			background.push_back(newCobble);
		}
	}

	healthBarBack = cocos2d::Sprite::create("healthbar.png", Rect(0, 0, 128, 32));
	healthBarBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	healthBarBack->setPosition(Vec2(50, 50));
	healthBarBack->setScale(4);
	healthBarFront = cocos2d::Sprite::create("healthbar.png", Rect(0, 32, 128, 32));
	healthBarFront->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	healthBarFront->setPosition(Vec2(50, 50));
	healthBarFront->setScale(4);

	auto label = cocos2d::Label::create();
	label->setName("HealthLabel");
	std::stringstream ss;
	ss << "Health: " << player->getHealth() << " / " << player->getMaxHealth();
	label->setString(ss.str());
	label->setSystemFontSize(50);
	label->setPosition(healthBarFront->getPosition() + healthBarFront->getBoundingBox().size / 2 + Vec2(0, 120));

	gui->addChild(healthBarBack, 100);
	gui->addChild(healthBarFront, healthBarBack->getLocalZOrder() + 1);
	gui->addChild(label, healthBarBack->getLocalZOrder() + 2);

	// ADD ALL GUI AND ACTORS BEFORE HERE
	this->scheduleUpdate();

	for (auto i : gui->getChildren())
		i->setPosition(i->getPosition() - cocos2d::Director::getInstance()->getVisibleSize() / 2);

	this->addChild(gui, 100);

	Retry::Camera::setPosition(player->getSprite()->getPosition());

	auto s = cocos2d::Sprite::create("CloseSelected.png");
	s->retain();
	s->setName("test");
	s->setPosition(0, -10000);
	this->addChild(s, 1000);

	level = new Retry::Level(1);
	this->addChild(level->getLevelDraw());

	Retry::Camera::lazyFollowTarget(player->getSprite(), 0.25f);
	Retry::Camera::setPosition(player->getPosition());
	Retry::Camera::setCamera(this->getDefaultCamera());

	Keyboard::createListener(_eventDispatcher, this);
	Mouse::createListener(_eventDispatcher, this);
	Controller::createListener(this);

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender) {
	cocos2d::Director::getInstance()->end();
}

void MainScene::update(float delta) {
	//if (Controller::isAxisPressed(ControllerButton::RIGHT_TRIGGER) ||
	//	Keyboard::isKeyPressed(KeyCode::SHIFT)) delta *= 0.25f;

	if (Keyboard::isKeyDown(KeyCode::ESCAPE) || Controller::isButtonDown(ControllerButton::START))
		cocos2d::Director::getInstance()->pushScene(OptionsMenu::createScene());

	if (Keyboard::isKeyDown(KeyCode::Q) || Controller::isButtonDown(ControllerButton::BACK))
		cocos2d::Director::getInstance()->replaceScene(MenuScene::create());

	if (Controller::isAxisPressed(ControllerButton::RIGHT_TRIGGER))
		delta *= 0.25f;

	if (Controller::isButtonPressed(ControllerButton::RIGHT_BUMPER))
		Retry::Camera::setTrauma(0.3f);

	if (Controller::isButtonPressed(ControllerButton::RIGHT_STICK))
		Retry::Camera::setTrauma(1);

	if (Keyboard::isKeyDown(KeyCode::F2) || Controller::isButtonDown(ControllerButton::LEFT_BUMPER))
		toggleDebug();
	//if (Controller::isAxisPressed(ControllerButton::LEFT_TRIGGER))
	Retry::Config::setDebug(Controller::isAxisPressed(ControllerButton::LEFT_TRIGGER));

	if (Keyboard::isKeyDown(KeyCode::F3))
		Retry::Config::toggleScreenShake();

	if (Keyboard::isKeyDown(KeyCode::F4))
		Retry::Config::toggleVibration();

	if (Keyboard::isKeyPressed(KeyCode::LEFT_ARROW))
		enemy->bufferAction("left");
	if (Keyboard::isKeyPressed(KeyCode::RIGHT_ARROW))
		enemy->bufferAction("right");
	if (Keyboard::isKeyPressed(KeyCode::UP_ARROW))
		enemy->bufferAction("jump");

	for (auto i : actorList) i->update(delta);
	static int count = 0;
	for (auto i : actorList) if (i != player) if (player->isAttackCollidingWith(i)) cocos2d::log("Hit! %d", count++);

	// COLLISION
	if (!Retry::Config::doDebug())
		player->doTerrainCollision(level, delta);
	for (auto i : actorList) if (i != player) i->doTerrainCollision(level, delta);

	if (Keyboard::isKeyPressed(KeyCode::UP_ARROW) ||
		Controller::isAxisPressed(ControllerButton::RIGHT_STICK_UP))
		player->increaseHealth(delta);
	else if (Keyboard::isKeyPressed(KeyCode::DOWN_ARROW) ||
			 Controller::isAxisPressed(ControllerButton::RIGHT_STICK_DOWN))
		player->decreaseHealth(delta);

	static float orgWidth = healthBarFront->getTextureRect().size.width;
	float newWidth = player->getHealthRatio() * orgWidth;
	healthBarFront->setTextureRect(Rect(Vec2(0, 32), Size(newWidth, healthBarFront->getTextureRect().size.height)));

	std::stringstream ss;
	ss << "Health: " << player->getHealth() << " / " << player->getMaxHealth();
	((cocos2d::Label*)gui->getChildByName("HealthLabel"))->setString(ss.str());

	Retry::Camera::update(delta);

	static bool doFull = false;
	if (Keyboard::isKeyDown(KeyCode::F11)) {
		if (!(doFull = !doFull))
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setWindowed(1280, 720);
		else
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
	}


	updateBackground();

	Retry::Camera::transformUI(gui);
}

void MainScene::initPlayer(cocos2d::Vec2 position) {
	player = new Retry::Player("cybercop.png", position);

	const cocos2d::Vec2 tileSize(128, 128);

	player->initAnimation("run", "cybercop.png", cocos2d::Vec2(0, 2), tileSize, 6);
	player->initAnimation("idle", "cybercop.png", cocos2d::Vec2(0, 0), tileSize, 20);
	player->initAnimation("jump", "cybercop.png", cocos2d::Vec2(6, 2), tileSize, 14);
	player->runAnimation("run", 0);

	//player->getHurtBox()->addRect(Vec2(32, 0), cocos2d::Size(64, 128));
	player->getHurtBox()->addCapsule(Vec2(64, 32), Vec2(64, 96), 32);
	//player->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 1));

	player->getSprite()->setScale(1);

	this->addChild(player->getSprite(), 100);
	actorList.emplace_back(player);
}

void MainScene::updateBackground() {
	static auto cSize = background.front()->getContentSize();
	static auto sSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 3; i++) {
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 3; j++) {
			int index = i + j * ((int) sSize.width / (int) cSize.width + 3);
			auto camPos = Retry::Camera::getPosition();
			background[index]->setPosition(camPos - sSize / 2 + cocos2d::Vec2((i - 1) * cSize.width, j * cSize.height) - cocos2d::Vec2((int) camPos.x % (int) cSize.width, (int) camPos.y % (int) cSize.height) + cocos2d::Vec2(10, 10));
		}
	}
}

void MainScene::toggleDebug() {
	//Retry::Config::toggleDebug();
	static bool doDraw = false;
	doDraw = !doDraw;
	for (auto i : actorList) {
		i->getHurtBox()->setDebugDraw(doDraw);
		i->getHitBox()->setDebugDraw(doDraw);
	}
}