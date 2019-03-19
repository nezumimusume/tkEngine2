#include "ListController.h"
#include "ViewConstants.h"

#define FONT "arial28.fnt"
#define LINE_SPACE          32
#define LEFT_MARGIN         40

using namespace cocos2d;

ListController* ListController::create(float width, float height, CCObject* target, ListControllerHandler selector)
{
	ListController* pRet = new ListController(width, height, target, selector);
	pRet->init();
	pRet->autorelease();

	return pRet;
}

ListController::ListController(float width, float height, CCObject* target, ListControllerHandler selector)
: beginTouchPosOffset(0)
, width(width)
, height(height)
, target(target)
, selector(selector)
{
	CCDrawNode* bg = CCDrawNode::create();
	bg->setPosition(0, 0);	
	CCPoint pp[] = {ccp(0, 0), ccp(width, 0), ccp(width, -height), ccp(0, -height)};
	bg->drawPolygon(pp, 4, ROOM_LIST_BG_COLOR, 0, GRID_COLOR);

	addChild(bg);
	menu = CCMenu::create();
	menu->setPosition(0, 0);	
	addChild(menu);
	setContentSize(CCSizeMake(width, height));
}

ListController::~ListController(void)
{
}

bool ListController::init(void)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	} while(0);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	scrollTo(0);
	return bRet;

}

bool ListController::addItem(const ExitGames::Common::JString& name)
{
	for(unsigned int i=0; i<menu->getChildrenCount(); ++i)
	{
		CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
		CCLabelBMFont* l = (CCLabelBMFont*)(item->getLabel());
		if(l->getString() == name)
			return false;
	}
	CCLabelBMFont* label = CCLabelBMFont::create(name.UTF8Representation().cstr(), FONT);
	label->setAlignment(cocos2d::kCCTextAlignmentLeft);
	CCMenuItemLabel* item = CCMenuItemLabel::create(label, this, menu_selector(ListController::menuCallback));

	menu->addChild(item);
	item->setPosition(ccp(LEFT_MARGIN, -(float)(menu->getChildrenCount() - 1) * LINE_SPACE - LINE_SPACE/2));
	item->setAnchorPoint(ccp(0, 0.5));

	CCDrawNode* border = CCDrawNode::create();
	border->setTag(1);
	border->setPositionX(-LEFT_MARGIN);
	item->addChild(border);

	scrollTo(menu->getPositionY());

	return true;
}

bool ListController::removeItem(const ExitGames::Common::JString& name)
{
	unsigned int i = 0;
	for(; i < menu->getChildrenCount(); ++i)
	{
		CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
		CCLabelBMFont* l = (CCLabelBMFont*)(item->getLabel());
		if(l->getString() == name)
		{
			item->removeFromParentAndCleanup(true);
			break;
		}
	}
	bool ret = false;
	if(i < menu->getChildrenCount())
	{
		for(; i<menu->getChildrenCount(); ++i)
		{
			CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
			item->setPositionY(item->getPositionY() + LINE_SPACE);
		}
		ret = true;
	}

	scrollTo(menu->getPositionY());

	return ret;
}

void ListController::removeAllItems(void)
{
	menu->removeAllChildrenWithCleanup(true);
	scrollTo(menu->getPositionY());
}

void ListController::menuCallback(CCObject* pSender)
{
	CCMenuItemLabel* item = (CCMenuItemLabel*)pSender;
	CCLabelBMFont* l = (CCLabelBMFont*)(item->getLabel());
	(target->*selector)(l->getString());
}

bool ListController::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint pos = convertToNodeSpace(touchLocation);
	if(pos.x >= 0 && pos.x < width && pos.y <= 0 && pos.y > -height)
	{
		beginTouchPosOffset = touch->getLocation().y - menu->getPositionY();
		return true;
	}
	else
		return false;
}

void ListController::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	float nextY = touchLocation.y - beginTouchPosOffset;
	scrollTo(nextY);
	beginTouchPosOffset += nextY - menu->getPositionY(); // scroll limits correction
}

void ListController::scrollTo(float nextY)
{
	// leave some space at bottom
	const float height2 = height - LINE_SPACE/3;
	if(nextY < 0.0f)
		menu->setPositionY(0);
	else 
	{
		float y = menu->getChildrenCount() * LINE_SPACE - height2;
		if(nextY > y)
			menu->setPositionY(y > 0 ? y : 0);
		else
			menu->setPositionY(nextY);
	}

	// clipping
	const float FADE_START = 0.9f;
	const float FADE_EXPANSION = 10;
	const float LABEL_CENTER_FIX = 4;
	for(unsigned int i=0; i<menu->getChildrenCount(); ++i)
	{
		CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
		float centerOffs = item->getPositionY() + (menu->getPositionY() + height2/2 + LABEL_CENTER_FIX);
		if(centerOffs < 0) centerOffs = -centerOffs;
		centerOffs = centerOffs / (height2/2 - LINE_SPACE/2 + FADE_EXPANSION);
		item->setEnabled(centerOffs < 1);
		unsigned char op = 0;
		if(centerOffs < FADE_START)
			op = 255;
		else if(centerOffs < 1) 
			op = static_cast<unsigned char>(255 * (1 - centerOffs) / (1 - FADE_START));
		else
			op = 0;
		item->setOpacity(op);

		CCDrawNode* border = (CCDrawNode*)item->getChildByTag(1);
		border->clear();
		float w = getContentSize().width;
		CCPoint pp[] = {ccp(0, 0), ccp(w, 0), ccp(w, 0), ccp(0, 0)};
		cocos2d::ccColor4F color = {1, 1, 1, op/512.0f};
		border->drawPolygon(pp, 4, TRANSPARENT_COLOR, 1, color);
	}
}