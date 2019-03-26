#include "OptionsMenu.h"

#include "KeyboardManager.h"
#include "MouseManager.h"
#include "AudioPlayer.h"
#include "ControllerManager.h"

#include "CameraManager.h"

#include "AudioEngine.h"

#include "GameSettings.h"

using cocos2d::Vec2;

cocos2d::Scene* OptionsMenu::createScene()
{
	return OptionsMenu::create();
}

void OptionsMenu::onExit()
{
	Scene::onExit();
}

void OptionsMenu::onEnter()
{
	Scene::onEnter();
}

bool OptionsMenu::init()
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

	initCheckBoxForSetting("Screen Shake");
	initCheckBoxForSetting("Vibration");

	for (const auto &i : widgets)
	{
		auto label = cocos2d::Label::create();
		label->setString(i->getName());
		label->setSystemFontSize(50);
		label->setPosition(i->getPosition() - Vec2(label->getContentSize().width + 50, 0));
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		gui->addChild(i);
		gui->addChild(label);
	}

	this->addChild(gui, 100);

	this->scheduleUpdate();

	return true;

	auto lmao = cocos2d::ui::CheckBox::create();
}

void OptionsMenu::update(float delta)
{
	if (cocos2d::Director::getInstance()->getRunningScene() == this &&
		Retry::Keyboard::isKeyDown(Retry::KeyCode::ESCAPE) ||
		Retry::Controller::isButtonDown(Retry::ControllerButton::B) ||
		Retry::Controller::isButtonDown(Retry::ControllerButton::START))
		//cocos2d::Director::getInstance()->popScene();
	{
		Retry::Audio::playEffect("sound/sound effects/buttonpress.mp3");
		DirectorEx* directorEx = static_cast<DirectorEx*>(cocos2d::Director::getInstance());
		Scene* prevScene = directorEx->previousScene();
		Scene* pScene = cocos2d::TransitionFadeBL::create(1.0f, prevScene);
		directorEx->popScene(pScene);
	}

}

void OptionsMenu::initCheckBoxForSetting(const std::string &s)
{
	using cocos2d::ui::CheckBox;

	if (!Retry::Config::isSetting(s)) return;

	auto box = CheckBox::create("CheckBoxes/CheckBox_Normal.png",
								"CheckBoxes/CheckBox_Press.png",
								"CheckBoxes/CheckBoxNode_Normal.png",
								"CheckBoxes/CheckBox_Disable.png",
								"CheckBoxes/CheckBoxNode_Disable.png");

	box->setName(s);

	box->setSelected(Retry::Config::doSetting(s));

	box->addEventListener([&](Ref* sender, CheckBox::EventType type) {
		Retry::Config::setSetting(((CheckBox*) sender)->getName(), !(bool) type);
	});

	box->setPosition(Vec2(1920 - 100, 1080 - 100 - 150 * widgets.size()));

	box->setScale(2);


	widgets.push_back(box);
	//toggleMenu->addSubItem(box);
}

cocos2d::Scene* DirectorEx::previousScene() {
	size_t sceneCount = _scenesStack.size();
	if (sceneCount <= 1) {
		return nullptr;
	}
	return _scenesStack.at(sceneCount - 2);
}

void DirectorEx::popScene(cocos2d::Scene* trans) {
	_scenesStack.popBack();
	size_t sceneCount = _scenesStack.size();
	if (sceneCount == 0) {
		end();
	} else {
		_sendCleanupToScene = true;
		_nextScene = trans;
	}
}