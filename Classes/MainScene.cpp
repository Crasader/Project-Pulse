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

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

#include "ui/CocosGUI.h"

#include "CollisionBody.h"

#include "Level.h"

using cocos2d::Vec2;

cocos2d::Scene* MainScene::createScene()
{
	auto scene = MainScene::create();

	return scene;
}

void MainScene::onExit()
{
	Scene::onExit();
}

void MainScene::onEnter()
{
	Scene::onEnter();
}

bool MainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	gui = cocos2d::Node::create();

	initPlayer(Vec2(1230, 350));
	actor = new Retry::Actor("CloseNormal.png", Vec2(1000, 50));
	this->addChild(actor->getSprite());
	actorList.push_back(actor);

	auto cobble = cocos2d::Sprite::create("cobblestone.png");
	auto cSize = cobble->getContentSize();
	auto sSize = cocos2d::Director::getInstance()->getVisibleSize();


	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 3; i++)
	{
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 3; j++)
		{
			auto newCobble = cocos2d::Sprite::create("cobblestone.png");
			newCobble->setAnchorPoint(Vec2(0, 0));
			newCobble->setPosition(i * cSize.width, j * cSize.height);
			newCobble->setColor(cocos2d::Color3B(127, 127, 127));
			this->addChild(newCobble, -100);
			background.push_back(newCobble);
		}
	}

	Retry::Camera::lazyFollowTarget(player->getSprite(), 0.25f);

	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	Retry::Controller::createListener(this);
	Retry::Camera::setCamera(this->getDefaultCamera());

	this->scheduleUpdate();



	for (auto i : gui->getChildren())
		i->setPosition(i->getPosition() - cocos2d::Director::getInstance()->getVisibleSize() / 2);

	this->addChild(gui, 100);

	Retry::Camera::setPosition(player->getSprite()->getPosition());

	player->getHurtBox()->addRect(Vec2(8, 0), cocos2d::Size(32, 40));
	//player->getHurtBox()->addRect(Vec2(0, 0), cocos2d::Size(48, 48));
	//player->getHurtBox()->addCircle(Vec2(24, 20), 20);
	//player->getHurtBox()->addCapsule(Vec2(50, 24), Vec2(-10, -10), 10);

	auto s = cocos2d::Sprite::create("CloseSelected.png");
	s->retain();
	s->setName("test");
	s->setPosition(0, -10000);
	this->addChild(s, 1000);

	level = new Retry::Level(1);
	this->addChild(level->getLevelDraw());
	//this->addChild(level->getDebugDraw());
	//level->getLevelDraw()->setScale(4);
	//level->getDebugDraw()->setScale(4);

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void MainScene::update(float delta)
{
	if (Retry::Controller::isAxisPressed(Retry::ControllerButton::RIGHT_TRIGGER) ||
		Retry::Keyboard::isKeyPressed(Retry::KeyCode::SHIFT)) delta *= 0.25f;

	if (Retry::Keyboard::isKeyDown(Retry::KeyCode::ESCAPE) || Retry::Controller::isButtonDown(Retry::ControllerButton::START))
		cocos2d::Director::getInstance()->replaceScene(MenuScene::createScene());

	if (Retry::Keyboard::isKeyDown(Retry::KeyCode::F2))
		toggleDebug();

	for (auto i : actorList) i->update(delta);


	// DO SIMPLE GRID COLLISION
	// CHECK IF A POSITION ON A GRID IS TRUE OR FALSE 
	if (player->doTerrainCollision(level, delta))
	{
		//player->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(0, 1, 0, 0.3f));
		//i->setDebugDrawColor(cocos2d::Color4F(0, 1, 0, 0.3f));
	} else
	{
		//player->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
		//i->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
	}

	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::UP_ARROW))
		this->setScale(2);
	else if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::DOWN_ARROW))
		this->setScale(0.5f);
	else this->setScale(1);

	//if (player->getHurtBox()->isCollidingWith(actor->getHurtBox()))
	//{
	//	player->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(0, 1, 0, 0.3f));
	//	actor->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(0, 1, 0, 0.3f));
	//} else
	//{
	//	player->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
	//	actor->getHurtBox()->setDebugDrawColor(cocos2d::Color4F(1, 0, 0, 0.3f));
	//}




	Retry::Camera::update(delta);

	static bool doFull = false;
	if (Retry::Keyboard::isKeyDown(Retry::KeyCode::F11))
	{
		if (!(doFull = !doFull))
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setWindowed(1280, 720);
		else
			dynamic_cast<cocos2d::GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
	}


	updateBackground();

	Retry::Camera::transformUI(gui);
}

void MainScene::initPlayer(cocos2d::Vec2 position)
{
	player = new Retry::Player("sonic.png", position);
	//player->getSprite()->setAnchorPoint(cocos2d::Vec2(0.5, 0));
	player->getSprite()->setScale(3);

	const cocos2d::Vec2 tileSize(48, 48);
	//player->getCollisionBody()->setSize(cocos2d::Size(tileSize));

	player->initAnimation("run", "sonic.png", cocos2d::Vec2(0, 0), tileSize, 6);
	player->initAnimation("idle", "sonic.png", cocos2d::Vec2(6, 0), tileSize, 1);
	player->initAnimation("jump", "sonic.png", cocos2d::Vec2(0, 3), tileSize, 5);
	player->runAnimation("run", 0);

	this->addChild(player->getSprite(), 100);
	actorList.push_back(player);
}

void MainScene::updateBackground()
{
	static auto cSize = background.front()->getContentSize();
	static auto sSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 3; i++)
	{
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 3; j++)
		{
			int index = i + j * ((int) sSize.width / (int) cSize.width + 3);
			auto camPos = Retry::Camera::getPosition();
			background[index]->setPosition(camPos - sSize / 2 + cocos2d::Vec2((i - 1) * cSize.width, j * cSize.height) - cocos2d::Vec2((int) camPos.x % (int) cSize.width, (int) camPos.y % (int) cSize.height) + cocos2d::Vec2(10, 10));
		}
	}
}

void MainScene::toggleDebug()
{
	doDebug = !doDebug;
	for (auto i : actorList)
	{
		i->getHurtBox()->setDebugDraw(doDebug);
		i->getHitBox()->setDebugDraw(doDebug);
	}
}