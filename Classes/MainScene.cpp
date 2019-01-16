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
//#include "SimpleAudioEngine.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioManager.h"
#include "ControllerManager.h"

//#include "AudioEngine.h"

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


static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MainScene::init() {
	if (!Scene::init()) {
		return false;
	}

	player = new Player("amheck.png", Vec2(1280 / 2, 720 / 2));
	//player->getSprite()->setScale(0.3f);
	this->addChild(player->getSprite(), 100);

	background = Sprite::create("HelloWorld.png");
	background->setAnchorPoint(Vec2(0.5f, 0.5f));
	background->setPosition(Vec2(1280 / 2, 720 / 2));
	this->addChild(background, 0);

	Retry::KeyboardManager::getInstance()->createListener(_eventDispatcher, this);
	Retry::MouseManager::getInstance()->createListener(_eventDispatcher, this);
	Retry::ControllerManager::getInstance()->createListener(_eventDispatcher, this);

	this->scheduleUpdate();
	

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();
}

void MainScene::update(float delta) {
	using namespace Retry;
	auto keyIn = KeyboardManager::getInstance();
	auto mouseIn = MouseManager::getInstance();
	auto controllerIn = ControllerManager::getInstance();
	auto audio = AudioManager::getInstance();

	if (keyIn->isKeyPressed(KeyCode::ESCAPE)) {
		Director::getInstance()->end();
	}

	//player->update(delta);

	//auto camera = this->getDefaultCamera();
	//camera->runAction(MoveBy::create(0, (player->getSprite()->getPosition() - camera->getPosition()) * move));

	player->getSprite()->setPosition(Vec2(1280 / 2, 720 / 2) + controllerIn->getLStick(0) * 200 + controllerIn->getLStick(1) * 50);

	if (controllerIn->isButtonDown(ControllerButton::A)) {
		log("A was pressed!");
	}


	audio->setMasterVolume(mouseIn->getY() / 720.f);

	keyIn->refresh();
	mouseIn->refresh();
	controllerIn->refresh();
}