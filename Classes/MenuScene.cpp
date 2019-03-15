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

	gui = Node::create();

	Retry::Keyboard::createListener(_eventDispatcher, this);
	Retry::Mouse::createListener(_eventDispatcher, this);
	Retry::Controller::createListener(this);

	auto cobble = cocos2d::Sprite::create("cobblestone.png");
	auto cSize = cobble->getContentSize();
	auto sSize = cocos2d::Director::getInstance()->getVisibleSize();

	for (int i = 0; i < (int) sSize.width / (int) cSize.width + 2; i++)
	{
		for (int j = 0; j < (int) sSize.height / (int) cSize.height + 2; j++)
		{
			auto newCobble = cocos2d::Sprite::create("cobblestone.png");
			newCobble->setAnchorPoint(Vec2(0, 0));
			newCobble->setPosition(i * cSize.width, j * cSize.height);
			newCobble->setColor(cocos2d::Color3B(127, 127, 127));
			this->addChild(newCobble);
		}
	}

	//auto mainMenu = cocos2d::Menu::create();

	//auto playItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", [&](Ref* sender) {
	//	cocos2d::Director::getInstance()->replaceScene(MainScene::createScene());
	//});
	//playItem->setScale(2);

	//auto closeItem = cocos2d::MenuItemImage::create("Close1.png", "Close2.png", [&](Ref* sender) {
	//	cocos2d::Director::getInstance()->end();
	//});
	//closeItem->setScale(0.25f);
	//closeItem->setPosition(Vec2(550, -300));


	//mainMenu->addChild(playItem);
	//mainMenu->addChild(closeItem);
	//gui->addChild(mainMenu);
	
	menu = new Retry::Menu(Vec2(960, 1080 / 5), this);
	menu->addButton("button.png", Size(256, 80), "New Game", [&]() {
		cocos2d::Director::getInstance()->replaceScene(MainScene::createScene());
	});
	menu->addButton("button.png", Size(256, 80), "Settings", [&]() {
		cocos2d::Director::getInstance()->pushScene(OptionsMenu::createScene());
	});
	menu->addButton("button.png", Size(256, 80), "Exit to Desktop", [&]() {
		cocos2d::Director::getInstance()->end();
	});
	//this->schedule(CC_CALLBACK_0(Retry::Menu::update, menu), "MenuUpdate");
	this->addChild(gui);

	this->scheduleUpdate();

	return true;
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();
}

void MenuScene::update(float delta)
{
	menu->update();
	//if (Retry::Keyboard::isKeyDown(Retry::KeyCode::ESCAPE) || 
	//	Retry::Controller::isButtonDown(Retry::ControllerButton::B))
	//	cocos2d::Director::getInstance()->end();

	//if (Retry::Controller::isButtonDown(Retry::ControllerButton::A) ||
	//	Retry::Keyboard::isKeyPressed(Retry::KeyCode::SPACE))
	//{
	//	cocos2d::Director::getInstance()->replaceScene(MainScene::createScene());
	//}


}