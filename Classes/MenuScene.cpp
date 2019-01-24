#include "MenuScene.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

#include "MainScene.h"

cocos2d::Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

void MenuScene::onExit()
{
	Scene::onExit();
}

void MenuScene::onEnter()
{
	Scene::onEnter();
}

bool MenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	Retry::Controller::createListener(_eventDispatcher, this);

	auto mainMenu = cocos2d::Menu::create();

	auto playItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", [&](Ref* sender) {
		cocos2d::Director::getInstance()->replaceScene(MainScene::createScene());
	});

	mainMenu->addChild(playItem);
	this->addChild(mainMenu);


	this->scheduleUpdate();

	return true;
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void MenuScene::update(float delta)
{
	if (Retry::Keyboard::isKeyPressed(Retry::KeyCode::ESCAPE)) cocos2d::Director::getInstance()->end();
	





}