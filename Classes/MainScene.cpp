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

	initPlayer(Vec2(1230, 50));

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
			this->addChild(newCobble);
			background.push_back(newCobble);
		}
	}

	for (auto i : cameraAnchors)
		this->addChild(i);
	Retry::Camera::lazyFollowTarget(player->getSprite(), 0.25f);
	for (auto i : cameraAnchors)
		Retry::Camera::addTarget(i);


	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	Retry::Controller::createListener(_eventDispatcher, this);

	this->scheduleUpdate();

	Retry::Camera::setCamera(this->getDefaultCamera());

	auto button = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", "CloseNormal.png");

	button->setCallback([&](Ref* sender) {
		static bool isPaused = false;

		if (!isPaused) this->unscheduleUpdate();
		else           this->scheduleUpdate();
		isPaused = !isPaused;
	});

	button->setPosition(Vec2(200, 200));

	auto b = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", "CloseNormal.png");

	b->setCallback([&](Ref* sender) {
		static bool isPaused = false;

		if (!isPaused) this->unscheduleUpdate();
		else           this->scheduleUpdate();
		isPaused = !isPaused;
	});

	b->setPosition(Vec2(200, 400));

	auto menu = cocos2d::Menu::create(button, b, NULL);
	menu->setPosition(cocos2d::Vec2(0, 0));

	gui->addChild(menu);
	for (auto i : gui->getChildren())
		i->setPosition(i->getPosition() - cocos2d::Director::getInstance()->getVisibleSize() / 2);

	this->addChild(gui, 100);

	Retry::Camera::setPosition(player->getSprite()->getPosition());

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void MainScene::update(float delta)
{
	if (Retry::Keyboard::isKeyDown(Retry::KeyCode::ESCAPE) || Retry::Controller::isButtonDown(Retry::ControllerButton::START))
	{
		cocos2d::Director::getInstance()->replaceScene(MenuScene::createScene());
	}

	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::E))
		Retry::Camera::setTrauma(0.5f);

	for (auto i : actorList) i->update(delta);
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

	transformUINodes();
}

void MainScene::initPlayer(cocos2d::Vec2 position)
{
	player = new Retry::Player("sonic.png", position);
	player->getSprite()->setAnchorPoint(cocos2d::Vec2(0.5, 0));
	player->getSprite()->setScale(3);

	player->initAnimation("run", "sonic.png", cocos2d::Vec2(0, 0), cocos2d::Vec2(48, 48), 6);
	player->initAnimation("idle", "sonic.png", cocos2d::Vec2(6, 0), cocos2d::Vec2(48, 48), 1);
	player->initAnimation("jump", "sonic.png", cocos2d::Vec2(0, 3), cocos2d::Vec2(48, 48), 5);
	player->runAnimation("run", 0.15f);

	this->addChild(player->getSprite(), 100);
	actorList.push_back(player);
}

void MainScene::transformUINodes()
{
	Retry::Camera::transformUI(gui);
	//for (auto i : gui->getChildren())
	//i->setScale(1920.f / cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().width);

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