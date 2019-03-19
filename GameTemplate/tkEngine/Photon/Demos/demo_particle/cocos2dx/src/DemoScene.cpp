#include "Console.h"
#include "DemoScene.h"
#include "ViewConstants.h"
#include "../../src/DemoConstants.h"

using namespace cocos2d;
using namespace ExitGames::Common;

#define Z_TOPMOST   100

#define LABEL_HEIGHT (2 * 15)

cocos2d::CCLabelBMFont* gpLabelLog = NULL;

class ConsoleImplementation
{
public:
	void write(const ExitGames::Common::JString& str)
	{
		gpLabelLog->setString(str.UTF8Representation().cstr());
	}

	void writeLine(const ExitGames::Common::JString& str)
	{
		write(str);
	}
};

Console::Console(void)
	: mpImp(new ConsoleImplementation)
{
}

Console::~Console(void)
{
	delete mpImp;
}

void Console::update(void)
{
}

void Console::write(const ExitGames::Common::JString& str)
{
	mpImp->write(str);
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
	mpImp->writeLine(str);
}

Console& Console::get(void)
{
	static Console console;
	return console;
}

typedef void (CCObject::*CheckBoxHandler)(bool);

// setContentSize does not work as expected, returning size by parameter
static CCMenuItem* decorateButton(CCMenuItemLabel* item, CCSize& size)
{
	const cocos2d::ccColor4F BORDER_COLOR = {1, 1, 1, 1};
	const cocos2d::ccColor4F NORMAL_COLOR = {1, 1, 1, 0.3f};

	CCDrawNode* bg = CCDrawNode::create();
	bg->setPosition(0, 0);
	float lw = item->getLabel()->getContentSize().width;
	float lh = item->getLabel()->getContentSize().height;
	float marginX = lh*0.4f;
	float marginY = lh*0.2f;
	float w = lw + 2*marginX;
	float h = lh + 2*marginY;
	CCPoint pp[] = {ccp(0, 0), ccp(w, 0), ccp(w, h), ccp(0, h)};
	bg->drawPolygon(pp, 4, NORMAL_COLOR, 1, BORDER_COLOR);
	bg->setPosition(-marginX, - marginY);
	item->addChild(bg);
	// adding another margin around rect
	size.width = w + marginX;
	size.height = h + marginY;
	return item;
}

static CCMenuItem* createButton(CCLabelBMFont* label, CCObject* target, SEL_MenuHandler menuHandler, CCSize& size)
{
	CCMenuItemLabel* item = CCMenuItemLabel::create(label, target, menuHandler);
	decorateButton(item, size);
	return item;
}

class CheckBox : public CCMenuItemLabel
{
public:
	static CCMenuItemLabel* create(CCLabelBMFont* label, CCObject* target, CheckBoxHandler selector, bool checked, CCSize& size)
	{
		CheckBox* pRet = new CheckBox();
		pRet->initCheckbox(label, target, selector);
		pRet->setChecked(checked);
		// init after label size updated
		pRet->initWithLabel(label, NULL, NULL);
		decorateButton(pRet, size);
		pRet->autorelease();

		return pRet;
	}
	virtual void activate(void)
	{	
		setChecked(!checked);
		(target->*selector)(checked);
	}
	void setChecked(bool v)
	{
		checked = v;
		label->setCString((checked ? uncheckedString + " X" : uncheckedString + "  ").c_str());
	}
	void initCheckbox(CCLabelBMFont* label, CCObject* target, CheckBoxHandler selector)
	{
		this->label = label;
		this->target = target;
		this->selector = selector;
		uncheckedString = label->getString();
		uncheckedString += ":";
	}
private:
	CheckBoxHandler selector;
	CCObject* target;
	CCLabelBMFont* label;
	bool checked;
	std::string uncheckedString;
};

DemoScene::DemoScene(void)
	: mpCanvas(NULL)
	, mpGrid(NULL) 
	, mpChecker(NULL)
	, mpPlayersNode(NULL)
	, mpLabelState(NULL)
	, mCanvasSize(1)
	, mCellSize(1)
{
}

DemoScene::~DemoScene(void)
{
}

DemoScene* DemoScene::create(void)
{
	DemoScene* pRet = new DemoScene();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
};

void DemoScene::onExit(void)
{
}

// on "init" you need to initialize your instance
bool DemoScene::init(void)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	} while(0);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	return bRet;
}

bool DemoScene::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	onTouch(touch, event);
    return true;
}

void DemoScene::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	onTouch(touch, event);
}

void DemoScene::onTouch(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
    CCPoint pos = mpCanvas->convertToNodeSpace(touchLocation);
	mpLbl->setLocalPlayerPos((int)pos.x/mCellSize, -(int)pos.y/mCellSize);
}

class MoveDesktopButton : public cocos2d::CCLayer
{
public:
	static MoveDesktopButton* create(float y, float height)
	{
		MoveDesktopButton* pRet = new MoveDesktopButton(y, height);
		pRet->init();
		pRet->autorelease();

		return pRet;
	}
	MoveDesktopButton(float y, float height) : height(height)
	{
		setPositionY(y);

		float width = CCDirector::sharedDirector()->getWinSize().width;
		CCDrawNode* bg = CCDrawNode::create();
		bg->setPosition(0, 0);	
		CCPoint pp[] = {ccp(0, 0), ccp(width, 0), ccp(width, -height), ccp(0, -height)};
		bg->drawPolygon(pp, 4, MOVE_DESKTOP_BUTTON_COLOR, 2, BLACK_COLOR);
		addChild(bg);

		CCLabelBMFont* l = CCLabelBMFont::create(": : : : : :", "arial28.fnt", width, cocos2d::kCCTextAlignmentCenter);
		l->setColor(ccc3(0, 0, 0));
		l->setPosition(width / 2, -height / 2);
		addChild(l);
	}
	//from cocos2d::CCLayer
	virtual bool init(void)
	{
		bool bRet = false;
		do
		{
			CC_BREAK_IF(! CCLayer::init());
			bRet = true;
		} while(0);

		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
		return bRet;
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		CCPoint touchLocation = touch->getLocation();    
		CCPoint pos = convertToNodeSpace(touchLocation);
		return pos.y <= 0 && pos.y > -height;
	}
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		float y = getParent()->getPositionY() + touch->getLocation().y - touch->getPreviousLocation().y;
		CCSize sz = CCDirector::sharedDirector()->getWinSize();
		if(y > 0 && y < getParent()->getContentSize().height - sz.height)
			getParent()->setPositionY(getParent()->getPositionY() + touch->getLocation().y - touch->getPreviousLocation().y);
	}
private:
	float height;
};

float DemoScene::setupDesktop(bool movable)
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	mCanvasSize = (int)(sz.width < sz.height ? sz.width : sz.height);

	float y = sz.height;

	mpLabelState = CCLabelBMFont::create("Undef", "arial28.fnt", (float)mCanvasSize, cocos2d::kCCTextAlignmentLeft);
	mpLabelState->setAnchorPoint(ccp(0, 1));
	mpLabelState->setPosition(0, sz.height);
	mpLabelState->setLineBreakWithoutSpace(true);
	addChild(mpLabelState);
		
	y -= mpLabelState->getContentSize().height;
	mpCanvas = CCDrawNode::create();
	mpCanvas->setPosition(0, y);

	CCPoint pp[] = {ccp(0, 0), ccp(mCanvasSize, 0), ccp(mCanvasSize, -mCanvasSize), ccp(0, -mCanvasSize)};
	mpCanvas->drawPolygon(pp, 4, CANVAS_COLOR, 0, CANVAS_COLOR);

	addChild(mpCanvas);
	mpPlayersNode = CCNode::create();
	mpCanvas->addChild(mpPlayersNode, Z_TOPMOST);

	y -= mCanvasSize;

	if(movable)
	{
		const float height = 60;
		addChild(MoveDesktopButton::create(y, height));
		y -= height;
	}
	CCSize bs;
	CCMenuItem* b0 = createButton(CCLabelBMFont::create("New Game", "arial28.fnt"), this, menu_selector(DemoScene::menuNewGameCallback), bs);
	CCMenuItem* b1 = createButton(CCLabelBMFont::create("Leave", "arial28.fnt"), this, menu_selector(DemoScene::menuLeaveCallback), bs);
	CCMenuItem* b2 = createButton(CCLabelBMFont::create("Grid Size", "arial28.fnt"), this, menu_selector(DemoScene::menuGridSizeCallback), bs);
	CCMenuItem* b3 = createButton(CCLabelBMFont::create("Random Color", "arial28.fnt"), this, menu_selector(DemoScene::menuRandomColorCallback), bs);
	CCMenuItem* b4 = CheckBox::create(CCLabelBMFont::create("Automove", "arial28.fnt"), this, (CheckBoxHandler)&DemoScene::menuAutomoveCallback, mpLbl->getAutomove(), bs);
	CCMenuItem* b5 = CheckBox::create(CCLabelBMFont::create("Groups", "arial28.fnt"), this, (CheckBoxHandler)&DemoScene::menuUseGroupsCallback, mpLbl->getUseGroups(), bs);
	float bh = bs.height;

	b0->setAnchorPoint(ccp(0, 0.5));
	b1->setAnchorPoint(ccp(1, 0.5));
	b2->setAnchorPoint(ccp(0, 0.5));
	b3->setAnchorPoint(ccp(1, 0.5));
	b4->setAnchorPoint(ccp(0, 0.5));
	b5->setAnchorPoint(ccp(1, 0.5));
	float marginX = bh*0.3f;
	b0->setPosition(ccp(marginX, -bh/2));
	b1->setPosition(ccp(mCanvasSize - marginX, -bh/2));
	b2->setPosition(ccp(marginX, -bh - bh/2));
	b3->setPosition(ccp(mCanvasSize - marginX, -bh - bh/2));
	b4->setPosition(ccp(marginX, -bh*2 - bh/2));
	b5->setPosition(ccp(mCanvasSize - marginX, -bh*2 - bh/2));

	CCMenu* pMenu = CCMenu::create(b0, b1, b2, b3, b4, b5, NULL);
	pMenu->setPosition(0, y);
	addChild(pMenu, Z_TOPMOST);

	y -= bh*3; // 3 button rows
	y -= bh*0.1f; // and some space
	
	float roomListHeight = (float)bh*4;
	mpRoomList = ListController::create(sz.width, roomListHeight, this, (ListControllerHandler)&DemoScene::roomListCallback);
	addChild(mpRoomList);
	mpRoomList->setPosition(0, y);
	mpRoomList->addItem("item-1");
	mpRoomList->addItem("item-2");
	mpRoomList->addItem("item-3");
	mpRoomList->addItem("item-4");
	mpRoomList->addItem("item-5");
	mpRoomList->addItem("item-6");
	mpRoomList->addItem("item-7");
	mpRoomList->addItem("item-8");

	y -= roomListHeight;

	gpLabelLog = CCLabelBMFont::create("Undef", "arial20.fnt", (float)mCanvasSize, cocos2d::kCCTextAlignmentLeft);
	gpLabelLog->setAnchorPoint(ccp(0, 1));
	gpLabelLog->setPosition(0, y);
	addChild(gpLabelLog);

	y -= gpLabelLog->getContentSize().height * 5; //extra lines for log

	return y;
}

void DemoScene::setupUI(void)
{
	if(setupDesktop(false) < 0)
	{
		removeAllChildrenWithCleanup(true);
		float extra = setupDesktop(true);
		CCSize sz = CCDirector::sharedDirector()->getWinSize();
		setContentSize(ccp(sz.width, sz.height - extra));
	}
	setupScene(DEFAULT_GRID_SIZE);
}

void DemoScene::setLBC(ExitGames::LoadBalancing::Client* lbc, LoadBalancingListener* lbl) 
{
	this->mpLbc = lbc;
	this->mpLbl = lbl;
}

void DemoScene::setupScene(int gridSize)
{
	mCellSize = mCanvasSize / gridSize;
	drawChecker(gridSize);
	drawGrid(gridSize);
	updatePlayerCellSizes(mCellSize);
}

void DemoScene::drawChecker(int gridSize)
{
	if(mpChecker)
		mpChecker->removeFromParentAndCleanup(true);
	mpChecker = CCDrawNode::create();
	mpChecker->setVisible(mpLbl->getUseGroups());
	mpCanvas->addChild(mpChecker);

	for(int x=0; x<GROUPS_PER_AXIS; ++x)
	{
		for(int y=0; y<GROUPS_PER_AXIS; ++y)
		{
			if((x%2) != (y%2)) 
			{
				float x0 = (float)mCellSize * (x *  gridSize / GROUPS_PER_AXIS);
				float y0 = -(float)mCellSize * (y * gridSize / GROUPS_PER_AXIS);
				float x1 = (float)mCellSize * ((x + 1) * gridSize / GROUPS_PER_AXIS);
				float y1 = -(float)mCellSize * ((y + 1) * gridSize / GROUPS_PER_AXIS);
				CCPoint pp[] = {ccp(x0, y0), ccp(x1, y0), ccp(x1, y1), ccp(x0, y1)};
				mpChecker->drawPolygon(pp, 4, CHECKER_COLOR, 0, CHECKER_COLOR);
			}
		}
	}
}

void DemoScene::drawGrid(int gridSize)
{
	if(mpGrid)
		mpGrid->removeFromParentAndCleanup(true);
	mpGrid = CCDrawNode::create();	
	mpCanvas->addChild(mpGrid);
	const float r = LINE_WIDTH/2.0f;
	for(int x=0; x<gridSize+1; ++x)
		mpGrid->drawSegment(ccp(x*mCellSize, - gridSize*mCellSize), ccp(x*mCellSize, 0), r, GRID_COLOR);

	for(int y=0; y<gridSize+1; ++y)
		mpGrid->drawSegment(ccp(0, - y*mCellSize), ccp(gridSize*mCellSize, - y*mCellSize), r, GRID_COLOR);
}

void DemoScene::updatePlayerCellSizes(int gridSize)
{
	for(unsigned int i=0; i<mpPlayersNode->getChildrenCount(); ++i)
	{
		PlayerView* p = (PlayerView*)mpPlayersNode->getChildren()->objectAtIndex(i);
		p->updateView(gridSize);
	}
}

void DemoScene::menuNewGameCallback(CCObject* pSender)
{
	mpLbl->createRoom();
}

void DemoScene::menuLeaveCallback(CCObject* pSender)
{
	if(mpLbc->getIsInGameRoom())
		mpLbc->opLeaveRoom();
	else
		CCDirector::sharedDirector()->popScene();
}

void DemoScene::menuGridSizeCallback(CCObject* pSender)
{
	mpLbl->nextGridSize();
}

void DemoScene::menuRandomColorCallback(CCObject* pSender)
{
	mpLbl->changeRandomColor();
}

void DemoScene::menuAutomoveCallback(bool automove)
{
	mpLbl->setAutomove(automove);
}

void DemoScene::menuUseGroupsCallback(bool useGroups)
{
	mpLbl->setUseGroups(useGroups);
	mpChecker->setVisible(useGroups);
	mpLbl->updateGroups();
}

void DemoScene::roomListCallback(const JString& name)
{
	mpLbc->opJoinRoom(name);
}

void DemoScene::updateState(int state, const JString& stateStr, const JString& joinedRoomName)
{
	mpLabelState->setString((JString() + state + L"/" + stateStr + joinedRoomName?L" "+joinedRoomName:L"").UTF8Representation().cstr());
}

void DemoScene::initPlayers(void) 
{
	mpPlayersNode->removeAllChildrenWithCleanup(true);
}

void DemoScene::addPlayer(int playerNr, const JString& playerName, bool local)
{
	removePlayer(playerNr);
	PlayerView* p = new PlayerView(playerNr, playerName, mCellSize, local ? 1.0f : 0);
	mpPlayersNode->addChild(p, 0, playerNr);
	p->setNumber(playerNr);
}

void DemoScene::removePlayer(int playerNr)
{
	mpPlayersNode->removeChildByTag(playerNr, true);
}

void DemoScene::changePlayerColor(int playerNr, int color)
{
	PlayerView* p = (PlayerView*)mpPlayersNode->getChildByTag(playerNr);
	if(p)
		p->setColor(color);
}

void DemoScene::changePlayerPos(int playerNr, int x, int y)
{
	PlayerView* p = static_cast<PlayerView*>(mpPlayersNode->getChildByTag(playerNr));
	if(p)
	{
		p->setPos(x, y);
		p->resetOpacity();
	}
}

void DemoScene::updateRoomList(const JVector<JString>& roomNames)
{
	mpRoomList->removeAllItems();
	for(unsigned int i=0; i<roomNames.getSize(); ++i)
		mpRoomList->addItem(roomNames[i]);
}

void DemoScene::tick(float interval)
{
	for(unsigned int i=0; i<mpPlayersNode->getChildrenCount(); ++i)
	{
		PlayerView* p = (PlayerView*)mpPlayersNode->getChildren()->objectAtIndex(i);
		p->fadeOpacity(interval / PLAYER_FADE_TIME);
	}
}