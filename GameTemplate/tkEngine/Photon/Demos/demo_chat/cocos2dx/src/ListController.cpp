#include "ListController.h"
#include "ViewConstants.h"

using namespace cocos2d;
using namespace ExitGames::Common;

// set according to screen size in ListController::init()
float LOG_FONT_SIZE       = 28;
float LOG_LINE_SPACE      = 32;
float LOG_LEFT_MARGIN     = 40;

const char* TOOLTIP_FNT_FILE1 = "arial20.fnt";
const char* TOOLTIP_FNT_FILE2 = "arial28.fnt";

CCLabelBMFont* tooltip = NULL;
CCDrawNode* tooltipBg = NULL;

void clearTooltip(void)
{
	if(tooltip) 
		tooltip->removeFromParentAndCleanup(true);
	if(tooltipBg) 
		tooltipBg->removeFromParentAndCleanup(true);
	tooltip = NULL;
	tooltipBg = NULL;
}

class LogMenuItemLabel : public CCMenuItemLabel
{
	virtual void selected(void)
	{
		CCMenuItemLabel::selected();
		
		clearTooltip();
//		CCLayer* layer = CCLayer::create();
	//	getParent()->getParent()->addChild(layer);
		float winWidth = CCDirector::sharedDirector()->getWinSize().width;
		const float MARGIN = 0.05f;
		float width = winWidth * (1 - 2 * MARGIN);
		tooltip = CCLabelBMFont::create(((CCLabelTTF*)getLabel())->getString(), winWidth < 400 ? TOOLTIP_FNT_FILE1 : TOOLTIP_FNT_FILE2, width);
		tooltip->setLineBreakWithoutSpace(true);
		tooltip->setAnchorPoint(ccp(0, 1));
		tooltipBg = CCDrawNode::create();
		float height = tooltip->getContentSize().height;
		float margin = width / 40;
		CCPoint pp[] = {ccp(-margin, margin), ccp(width + margin, margin), ccp(width + margin, -height - margin), ccp(-margin, -height - margin)};
		tooltipBg->drawPolygon(pp, 4, LOG_TOOLTIP_COLOR, 1, LOG_BORDER_COLOR);
		float y = convertToWorldSpace(ccp(0,0)).y;
		float y2 = y + height + FONT_SIZE * 2;

		CCNode* scene = getParent()->getParent()->getParent();
		if(y2 > CCDirector::sharedDirector()->getWinSize().height - margin)
			y2 = y - FONT_SIZE * 2;
		// attach to scene
		scene->addChild(tooltipBg);
		tooltipBg->setPosition(winWidth * MARGIN, y2 - scene->getPositionY());
		tooltipBg->addChild(tooltip);
	}

	virtual void unselected(void)
	{
		CCMenuItemLabel::unselected();
		
		clearTooltip();
	}
};

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
	bg->drawPolygon(pp, 4, LOG_COLOR, 1, LOG_BORDER_COLOR);

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

	float h = CCDirector::sharedDirector()->getWinSize().height;
	
	LOG_FONT_SIZE       = h * 3.3f / 100;
	LOG_LINE_SPACE      = LOG_FONT_SIZE * 8  /7;
	LOG_LEFT_MARGIN     = h / 20;

	return bRet;

}

CCMenuItemLabel * createLogMenuItemLabel(CCNode*label, CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItemLabel *pRet = new LogMenuItemLabel();
    pRet->initWithLabel(label, target, selector);
    pRet->autorelease();
    return pRet;
}

bool ListController::addItem(const JString& name)
{
	CCLabelTTF* label = CCLabelTTF::create(name.UTF8Representation().cstr(), FONT_NAME, LOG_FONT_SIZE);
    CCMenuItemLabel* item = createLogMenuItemLabel(label, this, menu_selector(ListController::menuCallback));
    menu->addChild(item);
	item->setPosition(ccp(LOG_LEFT_MARGIN, -(float)(menu->getChildrenCount() - 1) * LOG_LINE_SPACE));
	item->setAnchorPoint(ccp(0, 1));

	scrollTo(menu->getPositionY());

	return true;
}

bool ListController::removeItem(const ExitGames::Common::JString& name)
{
	unsigned int i = 0;
	for(; i<menu->getChildrenCount(); ++i)
	{
		CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
		CCLabelTTF* l = (CCLabelTTF*)(item->getLabel());
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
			item->setPositionY(item->getPositionY() + LOG_LINE_SPACE);
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

void ListController::scrollToBottom(void)
{
	scrollTo((float)menu->getChildrenCount() * LOG_LINE_SPACE);
}

void ListController::menuCallback(CCObject* pSender)
{
    CCMenuItemLabel* item = (CCMenuItemLabel*)pSender;
	CCLabelTTF* l = (CCLabelTTF*)(item->getLabel());
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
	const float height2 = height - LOG_LINE_SPACE/3;
    if(nextY < 0.0f)
    {
        menu->setPositionY(0);
    }
	else 
	{
		float y = menu->getChildrenCount() * LOG_LINE_SPACE - height2;
		if(nextY > y)
			menu->setPositionY(y > 0 ? y : 0);
		else
			menu->setPositionY(nextY);
	}

	// clipping
	const float FADE_START = 0.9f;
	const float FADE_EXPANSION = 10.f;
	const float LABEL_CENTER_FIX = 4.f;
	for(unsigned int i=0; i<menu->getChildrenCount(); ++i)
	{
		CCMenuItemLabel* item = (CCMenuItemLabel*)(menu->getChildren()->objectAtIndex(i));
		float centerOffs = item->getPositionY() + (menu->getPositionY() + height2/2 - LOG_LINE_SPACE/2 + LABEL_CENTER_FIX);
		if(centerOffs < 0) centerOffs = -centerOffs;
		centerOffs = centerOffs / (height2/2 - LOG_LINE_SPACE/2 + FADE_EXPANSION);
		item->setEnabled(centerOffs < 1);
		if(centerOffs < FADE_START)
			item->setOpacity(255);
		else if(centerOffs < 1) 
			item->setOpacity(GLbyte(255 * (1 - centerOffs) / (1 - FADE_START)));
		else
			item->setOpacity(0);
	}
}