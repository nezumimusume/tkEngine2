#pragma once

#include "cocos2d.h"
#include "Common-cpp/inc/Common.h"

typedef void (cocos2d::CCObject::*ListControllerHandler)(const ExitGames::Common::JString&);

class ListController : public cocos2d::CCLayer
{
public:
	static ListController* create(float width, float height, cocos2d::CCObject* target, ListControllerHandler selector);

	ListController(float width, float height, cocos2d::CCObject* target, ListControllerHandler selector);
    ~ListController(void);

	bool addItem(const ExitGames::Common::JString& label);
	bool removeItem(const ExitGames::Common::JString& label);
	void removeAllItems(void);

    void menuCallback(cocos2d::CCObject* pSender);

	//from cocos2d::CCLayer
	virtual bool init(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
private:
	void scrollTo(float moveY);
    float beginTouchPosOffset;
    cocos2d::CCMenu* menu;
	cocos2d::CCObject* target;
	ListControllerHandler selector;
	float width;
	float height;
};