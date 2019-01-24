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

cocos2d::Scene* MainScene::createScene()
{
	auto scene = MainScene::create();

	return MainScene::create();
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

	initPlayer(cocos2d::Vec2(1230, 50));

	auto cobble = cocos2d::Sprite::create("cobblestone.png");
	auto cSize = cobble->getContentSize();
	auto sSize = cocos2d::Director::getInstance()->getVisibleSize();


	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 2; i++)
	{
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 2; j++)
		{
			auto newCobble = cocos2d::Sprite::create("cobblestone.png");
			newCobble->setAnchorPoint(cocos2d::Vec2(0, 0));
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

	//this->schedule([=](float delta) { player->update(delta); }, "Player");

	//this->setScale(0.25);

	auto button = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png", "CloseNormal.png");

	button->addClickEventListener([&](Ref* sender) {
		static bool isPaused = false;

		if (!isPaused) this->unscheduleUpdate();
		else           this->scheduleUpdate();
		isPaused = !isPaused;
	});

	guiPos[button] = cocos2d::Vec2(1180, 100);
	//button->setPosition(cocos2d::Vec2(500, 200));

	this->addChild(button, 100);



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

	static auto cSize = background.front()->getContentSize();
	static auto sSize = cocos2d::Director::getInstance()->getVisibleSize();
	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 2; i++)
	{
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 2; j++)
		{
			int index = i + j * ((int) sSize.width / (int) cSize.width + 2);
			auto camPos = Retry::Camera::getPosition();
			background[index]->setPosition(camPos - sSize / 2 + cocos2d::Vec2((i - 1) * cSize.width, j * cSize.height) - cocos2d::Vec2((int) camPos.x % (int) cSize.width, (int) camPos.y % (int) cSize.height) + cocos2d::Vec2(10, 10));
		}
	}

	transformUINodes();
}

void MainScene::initPlayer(cocos2d::Vec2 position)
{
	player = new Retry::Player("CloseNormal.png", position);
	player->getSprite()->setScale(1.2f);
	this->addChild(player->getSprite(), 100);
	actorList.push_back(player);
}

void MainScene::transformUINodes()
{
	for (auto i : guiPos) Retry::Camera::transformUI(i.first, i.second);
}