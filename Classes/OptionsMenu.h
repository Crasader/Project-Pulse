#ifndef OPTIONS_MENU
#define OPTIONS_MENU

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>

class OptionsMenu : public cocos2d::Scene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(OptionsMenu);

	static cocos2d::Scene* createScene();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();

	void update(float delta);

private:
	cocos2d::Sprite* background;

	cocos2d::Node* gui;

	std::vector<cocos2d::ui::Widget*> widgets;

	void initCheckBoxForSetting(const std::string &s);
};

#endif