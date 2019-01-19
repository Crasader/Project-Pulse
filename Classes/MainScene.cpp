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
#include "AudioManager.h"
#include "ControllerManager.h"

#include "CameraManager.h"

USING_NS_CC;

Scene* MainScene::createScene() {
	return MainScene::create();
}

void MainScene::onExit() {
	Scene::onExit();
}

void MainScene::onEnter() {
	experimental::AudioEngine::lazyInit();
	Scene::onEnter();
}

bool MainScene::init() {
	if (!Scene::init()) {
		return false;
	}

	player = new Player("CloseNormal.png", Vec2(1230, 50));
	player->getSprite()->setScale(1.2f);
	this->addChild(player->getSprite(), 100);

	background = Sprite::create("HelloWorld.png");
	background->setAnchorPoint(Vec2(0.5f, 0.5f));
	background->setPosition(Vec2(1280 / 2, 720 / 2));
	this->addChild(background, 0);

	Retry::KeyboardManager::getInstance()->createListener(_eventDispatcher, this);
	Retry::MouseManager::getInstance()->createListener(_eventDispatcher, this);
	Retry::ControllerManager::getInstance()->createListener(_eventDispatcher, this);

	this->scheduleUpdate();

	CameraManager::getInstance()->setCamera(this->getDefaultCamera());

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();
}

void MainScene::update(float delta) {

	using namespace Retry;
	static auto keyIn = KeyboardManager::getInstance();
	static auto mouseIn = MouseManager::getInstance();
	static auto controllerIn = ControllerManager::getInstance();
	static auto audio = AudioManager::getInstance();


	if (keyIn->isKeyPressed(KeyCode::ESCAPE)) {
		Director::getInstance()->end();
	}

	player->update(delta);

	CameraManager::getInstance()->lazyFollowTarget(player->getSprite(), 0.25f);
	//CameraManager::getInstance()->moveBy((player->getSprite()->getPosition() - CameraManager::getInstance()->getPosition()) * delta * 4);

	if (keyIn->isKeyPressed(KeyCode::E))
		CameraManager::getInstance()->setTrauma(0.3f);

	if (keyIn->isKeyPressed(KeyCode::R))
		CameraManager::getInstance()->lazyFollowTarget(0);
	
	//camera->runAction(MoveBy::create(0, (player->getSprite()->getPosition() - camera->getPosition()) * delta * 4));

	//audio->setMasterVolume(mouseIn->getY() / 720.f);

	keyIn->refresh();
	mouseIn->refresh();
	controllerIn->refresh();
	CameraManager::getInstance()->update(delta);
}