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

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

cocos2d::Scene* MainScene::createScene()
{
	return MainScene::create();
}

void MainScene::onExit()
{
	Scene::onExit();
}

void MainScene::onEnter()
{
	cocos2d::experimental::AudioEngine::lazyInit();
	Scene::onEnter();
}

bool MainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	player = new Retry::Player("CloseNormal.png", cocos2d::Vec2(1230, 50));
	player->getSprite()->setScale(1.2f);
	this->addChild(player->getSprite(), 100);

	background = cocos2d::Sprite::create("HelloWorld.png");
	background->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	background->setPosition(cocos2d::Vec2(1280 / 2, 720 / 2));
	this->addChild(background, 0);

	cameraAnchors.push_back(cocos2d::Sprite::create("CloseSelected.png"));
	cameraAnchors.back()->setAnchorPoint(cocos2d::Vec2(0.5, 0.5f));
	cameraAnchors.back()->setPosition(cocos2d::Vec2(-500, -200));

	cameraAnchors.push_back(cocos2d::Sprite::create("CloseSelected.png"));
	cameraAnchors.back()->setAnchorPoint(cocos2d::Vec2(0.5, 0.5f));
	cameraAnchors.back()->setPosition(cocos2d::Vec2(1800, -150));

	cameraAnchors.push_back(cocos2d::Sprite::create("CloseSelected.png"));
	cameraAnchors.back()->setAnchorPoint(cocos2d::Vec2(0.5, 0.5f));
	cameraAnchors.back()->setPosition(cocos2d::Vec2(-300, 500));

	//this->addChild(cameraAnchors.front());
	//CameraManager::getInstance()->lazyFollowTarget(player->getSprite(), 0.25f);
	////for (auto i : cameraAnchors)
	//CameraManager::getInstance()->addTarget(cameraAnchors.front());

	for (auto i : cameraAnchors)
		this->addChild(i);
	Retry::Camera::lazyFollowTarget(player->getSprite(), 0.25f);
	for (auto i : cameraAnchors)
		Retry::Camera::addTarget(i);


	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	//Retry::ControllerManager::getInstance()->createListener(_eventDispatcher, this);
	Retry::ControllerManager::createListener(_eventDispatcher, this);

	this->scheduleUpdate();

	Retry::Camera::setCamera(this->getDefaultCamera());

	//this->setScale(0.25);

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void MainScene::update(float delta)
{

	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::ESCAPE))
	{
		cocos2d::Director::getInstance()->end();
	}

	player->update(delta);

	//CameraManager::getInstance()->moveBy((player->getSprite()->getPosition() - CameraManager::getInstance()->getPosition()) * delta * 4);

	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::E))
		Retry::Camera::setTrauma(0.5f);

	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::LEFT_ARROW))
		cameraAnchors.front()->runAction(cocos2d::MoveBy::create(0, cocos2d::Vec2(-500 * delta, 0)));
	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::RIGHT_ARROW))
		cameraAnchors.front()->runAction(cocos2d::MoveBy::create(0, cocos2d::Vec2(500 * delta, 0)));

	//if (keyIn->isKeyPressed(KeyCode::R) || controllerIn->isAxisPressed(ControllerButton::RIGHT_TRIGGER))
	//	CameraManager::getInstance()->lazyFollowTarget(0);

	Retry::Keyboard::refresh();
	Retry::Mouse::refresh();
	Retry::ControllerManager::refresh();
	Retry::Camera::update(delta);

}