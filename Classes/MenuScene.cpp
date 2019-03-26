#include "MenuScene.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

#include "MainScene.h"

#include "Menu.h"

#include "OptionsMenu.h"

using cocos2d::Vec2;
using cocos2d::Size;

cocos2d::Scene* MenuScene::createScene() {
	return MenuScene::create();
}

void MenuScene::onExit() {
	Scene::onExit();
}

void MenuScene::onEnter() {
	Scene::onEnter();
}

bool MenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	Retry::Audio::stopMusic();
	Retry::Audio::playMusic("sound/music/menu.mp3", true, 0.5f);

	gui = Node::create();

	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	Retry::Controller::createListener(this);

	backGround = new Retry::MenuActor("title_background.png", { 960, 1080 / 2 }, { 80, 45 }, 60);
	backGround->getSprite()->setScale(1080.0f / 42.0f);
	gui->addChild(backGround->getSprite());

	title = new Retry::MenuActor("title_title.png", { 50, 1040 }, { 64, 32 }, 60);
	title->getSprite()->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
	title->getSprite()->setScale(8.0f * 0.7f);
	gui->addChild(title->getSprite());

	menu = new Retry::Menu(Vec2(230, 700), gui);
	menu->addButton("startbutton.png", Size(64, 32), [&]() {
		cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFadeTR::create(1.0f, MainScene::createScene()));
	});
	menu->addButton("optionsbutton.png", Size(64, 32), [&]() {
		cocos2d::Director::getInstance()->pushScene(cocos2d::TransitionFadeTR::create(1.0f, OptionsMenu::createScene()));
	});
	menu->addButton("exitbutton.png", Size(64, 32), [&]() {
		cocos2d::Director::getInstance()->end();
	});
	menu->setScale(8.0f * 0.7f);
	menu->setPadding(10);

	this->addChild(gui);

	this->scheduleUpdate();

	return true;
}

void MenuScene::menuCloseCallback(Ref* pSender) {
	cocos2d::Director::getInstance()->end();
}

void MenuScene::update(float delta) {
	menu->update();
	backGround->update(delta);
	title->update(delta);
}