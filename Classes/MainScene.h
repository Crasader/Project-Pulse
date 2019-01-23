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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Player.h"

class MainScene : public cocos2d::Scene {
public:
	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);
	
	static cocos2d::Scene* createScene();
	virtual void onExit();
	virtual void onEnter(); 
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float delta);

	//void onConnected(Controller* controller, Event* event);
	//void onDisconnected(Controller* controller, Event* event);
	//void onKeyDown(Controller* controller, int key, Event* event);
	//void onKeyUp(Controller* controller, int key, Event* event);
	//void onKeyRepeat(Controller* controller, int key, Event* event);
	//void onAxisEvent(Controller* controller, int key, Event* event);


private:

	//Layer* background;
	cocos2d::Layer* foreground;

	Retry::Player* player;

	cocos2d::Sprite* background;

	std::vector<cocos2d::Sprite*> cameraAnchors;
};

#endif // __HELLOWORLD_SCENE_H__
