#ifndef MENU_SCENE
#define MENU_SCENE

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

	static cocos2d::Scene* createScene();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float delta);

private:
	cocos2d::Sprite* background;

	cocos2d::Node* gui;
};

#endif