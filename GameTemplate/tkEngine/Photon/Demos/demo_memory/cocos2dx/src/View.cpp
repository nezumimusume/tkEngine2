#include "View.h"
#include "ViewConstants.h"
#include "../../src/DemoConstants.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#include <math.h>

using namespace cocos2d;
using namespace ExitGames::Common;

#define Z_TOPMOST   100

const char* tileIconFiles[] =
{
"tile-angler-fish.png",
"tile-crown.png",
"tile-drink-me.png",
"tile-fishbone.png",
"tile-glass-heart.png",
"tile-horse-head.png",
"tile-robe.png",
"tile-rocket.png",
};

const char* FONT_MEDIUM = "arial28.fnt";
const char* FONT_SMALL = "arial20.fnt";

// setContentSize does not work as expected, returning size by parameter
static CCMenuItem* decorateButton(CCMenuItemLabel* item, CCSize& size)
{
	const cocos2d::ccColor4F BORDER_COLOR = {1, 1, 1, 0.5};
	const cocos2d::ccColor4F NORMAL_COLOR = {1, 1, 1, 0.3f};

	CCDrawNode* bg = CCDrawNode::create();
	bg->setPosition(0, 0);
	float lw = item->getLabel()->getContentSize().width;
	float lh = item->getLabel()->getContentSize().height;
	float marginX = lh*0.2f;
	float marginY = lh*0.05f;
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

View::View(void)
	: mpLbl(NULL)
	, mBoardSize(0)
	, mTileSize(0)
	, mTileMargin(0)
	, mTileRows(0)
	, mTileCols(0)
	, mpSceneConnect(NULL)
	, mpSceneLobby(NULL)
	, mpSceneGame(NULL)
	, mpCurrentScene(NULL)
	, mpBoard(NULL)
	, mpTiles(NULL) 
	, mpLabelState(NULL)
	, mpLabelLog(NULL)
	, mpRoomList(NULL)
	, mpSavedRoomList(NULL)
	, mpLobbyStats1(NULL)
	, mpLobbyStats2(NULL)
	, mpLabelMessage(NULL)
	, mStateString(JString())
	, mpInputUserName(NULL)
{
}

View::~View(void)
{
}

View* View::create(void)
{
	View* pRet = new View();
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

void View::init(LoadBalancingListener* lbl) 
{
	this->mpLbl = lbl;
#if CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE
	setKeypadEnabled(true);
#endif
}

void View::onExit(void)
{
}

// on "init" you need to initialize your instance
bool View::init(void)
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

bool View::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();
	CCPoint pos = mpBoard->convertToNodeSpace(touchLocation);
	return pos.x > 0 && pos.x < mBoardSize && pos.y > -mBoardSize && pos.y < 0;
}

void View::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void View::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint pos = mpBoard->convertToNodeSpace(touchLocation);
	int x = (int)(pos.x / (mTileSize + 2 * mTileMargin));
	int y = (int)((mBoardSize + pos.y) / (mTileSize + 2 * mTileMargin));
	if(x >= 0 && x < mTileCols && y >= 0 && y < mTileRows)
		mpLbl->click(x + y * mTileCols);
}

void View::setup(void)
{
    setupUI();
    EGLOG(DebugLevel::INFO, L"setup");
    switchScene(mpSceneConnect);
}

void View::setupUI(void)
{
	CCMenuItem *b0, *b1, *b2, *b3;
	CCMenu* menu;
	float y;

	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	mBoardSize = sz.width < sz.height ? sz.width : sz.height;
	mTileMargin = ceilf(sz.height / 200);

	// common controls
		
	mpLabelState = CCLabelBMFont::create("Undef", FONT_SMALL, (float)mBoardSize, cocos2d::kCCTextAlignmentLeft);
	mpLabelState->setLineBreakWithoutSpace(true);
	mpLabelLog = CCLabelBMFont::create("Undef", FONT_SMALL, (float)mBoardSize, cocos2d::kCCTextAlignmentLeft);
	addChild(mpLabelState);
	addChild(mpLabelLog);
	mpLabelState->setAnchorPoint(ccp(0, 1));
	mpLabelLog->setAnchorPoint(ccp(0, 1));
	mpLabelState->setPosition(0, sz.height);                                 // state docked to screen top
	mpLabelLog->setPosition(0, mpLabelLog->getContentSize().height * 4);	   // log docked to screen bottom

	// scenes
	float y0 = sz.height - mpLabelState->getContentSize().height;
	mpSceneConnect = CCNode::create();
	mpSceneLobby = CCNode::create();
	mpSceneGame = CCNode::create();
	addChild(mpSceneConnect);
	addChild(mpSceneLobby);
	addChild(mpSceneGame);
	mpSceneConnect->setPositionY(y0);
	mpSceneLobby->setPositionY(y0);
	mpSceneGame->setPositionY(y0);

	// mpSceneConnect
	y = - sz.height / 3;
	CCLabelBMFont* l = CCLabelBMFont::create("User:", FONT_MEDIUM);
	float lw = l->getContentSize().width;
	float lh = l->getContentSize().height;
	float marginX = lh*0.3f;
	float marginY = lh*0.15f;
	l->setAnchorPoint(ccp(0, 0.5f));
	l->setPosition(marginX, y);
	mpSceneConnect->addChild(l);

#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
	mpInputUserName = CCLabelBMFont::create(DEFAULT_USER_NAME.ANSIRepresentation().cstr(), FONT_MEDIUM);
	mpSceneConnect->addChild(mpInputUserName);
	mpInputUserName->setAnchorPoint(ccp(0, 0.5f));
	mpInputUserName->setPosition(marginX + lw + marginX, y);    
#else
	mpInputUserName = CCTextFieldTTF::textFieldWithPlaceHolder("<set user name>", "", 28);
	mpInputUserName->setDelegate(this);
	mpInputUserName->setString(DEFAULT_USER_NAME.ANSIRepresentation().cstr());
	mpInputUserName->setColor(INPUT_COLOR);
	CCMenuItemLabel* inputUserNameButton = CCMenuItemLabel::create(mpInputUserName, this, menu_selector(View::menuInputUserNameTouchCallback));
	inputUserNameButton->setAnchorPoint(ccp(0, 0.5f));
	inputUserNameButton->setPosition(ccp(marginX + lw + marginX, y));    
	CCMenu* inputUserNameMenu = CCMenu::create(inputUserNameButton, NULL);
	inputUserNameMenu->setAnchorPoint(ccp(0, 0));
	inputUserNameMenu->setPosition(0,0);
	mpSceneConnect->addChild(inputUserNameMenu);

#endif
	y -= 2*lh;

	CCSize bs;
	b0 = createButton(CCLabelBMFont::create("Connect", FONT_MEDIUM), this, menu_selector(View::menuConnectCallback), bs);
	menu = CCMenu::create(b0, NULL);
	menu->setPosition(0, 0);
	b0->setAnchorPoint(ccp(0.5f, 0.5f));
	b0->setPosition(mBoardSize/2, y);  y -= bs.height;
	mpSceneConnect->addChild(menu);

	// mpSceneLobby
	y = 0;
	float roomListHeight = sz.height / 4;
	mpLobbyStats1 = CCLabelBMFont::create("Saved Games:", FONT_MEDIUM);	
	mpSavedRoomList = ListController::create(sz.width, roomListHeight, this, (ListControllerHandler)&View::savedRoomListCallback);
	b0 = createButton(CCLabelBMFont::create("Refresh", FONT_MEDIUM), this, menu_selector(View::menuRefreshCallback), bs);
	b1 = createButton(CCLabelBMFont::create("Leave", FONT_MEDIUM), this, menu_selector(View::menuLeaveCallback), bs);
	mpLobbyStats2 = CCLabelBMFont::create("Rooms in lobby:", FONT_MEDIUM);
	mpRoomList = ListController::create(sz.width, roomListHeight, this, (ListControllerHandler)&View::roomListCallback);
	b2 = createButton(CCLabelBMFont::create("Random", FONT_MEDIUM), this, menu_selector(View::menuRandomGameCallback), bs);
	b3 = createButton(CCLabelBMFont::create("New Game", FONT_MEDIUM), this, menu_selector(View::menuNewGameCallback), bs);
	mpLobbyStats1->setAnchorPoint(ccp(0, 1));
	mpLobbyStats2->setAnchorPoint(ccp(0, 1));
	b0->setAnchorPoint(ccp(0, 0.5f));
	b1->setAnchorPoint(ccp(1, 0.5f));
	b2->setAnchorPoint(ccp(0, 0.5f));
	b3->setAnchorPoint(ccp(1, 0.5f));
	float bh = bs.height;
	float bsh2 = bs.height/2;
	mpLobbyStats1->setPositionY(y);                                     y -= mpLobbyStats1->getContentSize().height;
	mpSavedRoomList->setPosition(0, y);                                 y -= roomListHeight + marginY;
	b0->setPosition(marginX, y - bsh2);  b1->setPosition(mBoardSize - marginX, y - bsh2);  y -= bh;
	mpLobbyStats2->setPositionY(y);                                     y -= mpLobbyStats2->getContentSize().height;
	mpRoomList->setPosition(0, y);                                      y -= roomListHeight + marginY;
	b2->setPosition(marginX, y - bsh2);  b3->setPosition(mBoardSize- marginX, y - bsh2);  y -= bh;
	
	menu = CCMenu::create(b0, b1, b2, b3, NULL);
	menu->setPosition(0, 0);
	
	mpSceneLobby->addChild(mpLobbyStats1);
	mpSceneLobby->addChild(mpSavedRoomList);	
	mpSceneLobby->addChild(mpLobbyStats2);
	mpSceneLobby->addChild(mpRoomList);
	mpSceneLobby->addChild(menu);
	
	// mpSceneGame
	y = 0;
	mpLabelMessage = CCLabelBMFont::create("Welcome", FONT_MEDIUM);
	mpLabelMessage->setAnchorPoint(ccp(0, 1));
	mpLabelMessage->setPositionY(y);           y -= mpLabelMessage->getContentSize().height;
	mpBoard = CCDrawNode::create();
	mpBoard->setPositionY(y);                  y -= mBoardSize;
	
	CCPoint pp[] = {ccp(0, 0), ccp(mBoardSize, 0), ccp(mBoardSize, -mBoardSize), ccp(0, -mBoardSize)};
	mpBoard->drawPolygon(pp, 4, BOARD_COLOR, 0, BOARD_COLOR);

	b0 = createButton(CCLabelBMFont::create("Suspend", FONT_MEDIUM), this, menu_selector(View::menuSuspendCallback), bs);
	b1 = createButton(CCLabelBMFont::create("Abandon", FONT_MEDIUM), this, menu_selector(View::menuAbandonCallback), bs);
	b0->setAnchorPoint(ccp(0, 0.5f));
	b1->setAnchorPoint(ccp(1, 0.5f));
	// bh = set above
	b0->setPosition(marginX, y - bsh2);
	b1->setPosition(mBoardSize - marginX, y - bsh2);

	menu = CCMenu::create(b0, b1, NULL);
	menu->setPosition(0, 0);

	mpSceneGame->addChild(mpBoard);
	mpSceneGame->addChild(mpLabelMessage);
	mpSceneGame->addChild(menu, Z_TOPMOST);

	mpSceneConnect->setPositionX(0);
	mpSceneLobby->setPositionX(sz.width);
	mpSceneGame->setPositionX(sz.width);
	mpCurrentScene = mpSceneConnect;

}

void View::menuInputUserNameTouchCallback(CCObject* pSender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
#else
	mpInputUserName->attachWithIME();
#endif
}

bool View::onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, const JString& text, int nLen)
{
	return false;
}

void View::keyBackClicked(void)
{
	CCDirector::sharedDirector()->popScene();
}

void View::menuLeaveCallback(CCObject* pSender)
{
	if(!mpLbl->leave())
		CCDirector::sharedDirector()->popScene();
}

const char* View::getInputUserName(void)
{
	return mpInputUserName->getString();
}

void View::menuConnectCallback(CCObject* pSender)
{
	mpLbl->connect(getInputUserName());
}

void View::menuNewGameCallback(CCObject* pSender)
{
	mpLbl->createRoom(getInputUserName());
}

void  View::menuRandomGameCallback(CCObject* pSender)
{
	mpLbl->joinRandomRoom();
}

void  View::menuRefreshCallback(CCObject* pSender)
{
	mpLbl->querySavedRoomList();
}

void  View::menuSuspendCallback(CCObject* pSender)
{
	mpLbl->suspendRoom();
}

void  View::menuAbandonCallback(CCObject* pSender)
{
	mpLbl->abandonRoom();
}

void View::roomListCallback(const JString& name)
{
	mpLbl->joinRoom(name);
}

void View::savedRoomListCallback(const JString& name)
{
	mpLbl->joinSavedRoom(name);
}

void View::onStateChange(int state, const JString& stateStr, bool inLobby, bool inRoom)
{
	mStateString = stateStr;

	refreshStateInfo(JString());
	if(inLobby)
		switchScene(mpSceneLobby);
	else if(inRoom)
		switchScene(mpSceneGame);
	else
		switchScene(mpSceneConnect);
}
void View::refreshStateInfo(const JString& stateInfo)
{
	mpLabelState->setString((stateInfo?(mStateString+L" "+stateInfo):mStateString).UTF8Representation().cstr());
}

void View::refreshMessage(const JString& msg)
{
	mpLabelMessage->setString(msg.UTF8Representation().cstr());
}

void View::updateRoomList(const JVector<JString>& roomNames)
{
	mpRoomList->removeAllItems();
	for(unsigned int i=0; i<roomNames.getSize(); ++i)
		mpRoomList->addItem(roomNames[i]);
}

void View::updateSavedRoomList(const JVector<JString>& savedRoomNames)
{
	mpSavedRoomList->removeAllItems();
	for(unsigned int i=0; i<savedRoomNames.getSize(); ++i)
		mpSavedRoomList->addItem(savedRoomNames[i]);
}

void View::setupBoard(int tileCols, int tileRows)
{
	this->mTileCols = tileCols;
	this->mTileRows = tileRows;
	EGLOG(DebugLevel::INFO, L"setupBoard", tileCols, tileRows);
	if(mpTiles)
		mpTiles->removeFromParentAndCleanup(true);
	mpTiles = CCNode::create();
	mpBoard->addChild(mpTiles, Z_TOPMOST);
	
	mTileSize = (mBoardSize - (tileCols * mTileMargin * 2)) / tileCols;
	for(int y = 0; y < tileRows;++y)
		for(int x = 0; x < tileCols;++x)
		{
			CCNode* tile = CCNode::create();
			tile->setPosition(mTileMargin + x * (mTileSize + 2 * mTileMargin), - (mTileMargin + ((tileRows - 1 - y) * (mTileSize + 2 * mTileMargin))));
			mpTiles->addChild(tile);
		}
}

void View::updateBoard(unsigned char* tileStates, unsigned char flippedTiles[2])
{
	for(int  i=0; i<mTileCols*mTileRows; ++i)
	{
		CCNode* tile = (CCNode*)mpTiles->getChildren()->objectAtIndex(i);
		unsigned char tileState = tileStates[i];
		bool flipped = flippedTiles[0] == i || flippedTiles[1] == i;
		if(tile->getChildrenCount() == 0)
		{
			CCDrawNode* rect = CCDrawNode::create();
			CCPoint pp[] = {ccp(0, 0), ccp(mTileSize, 0), ccp(mTileSize, -mTileSize), ccp(0, -mTileSize)};
			rect->drawPolygon(pp, 4, HIDDEN_TILE_COLOR, 0, HIDDEN_TILE_COLOR);
			rect->setAnchorPoint(ccp(0, 1));
			CCNode* icon;
			if(tileState)
			{
				icon = CCSprite::create(tileIconFiles[tileState - 1]);
				icon->setScale(mTileSize / icon->getContentSize().width);
				icon->setAnchorPoint(ccp(0, 1));
			}
			else
				icon = CCNode::create(); //dummy

			tile->addChild(rect);
			tile->addChild(icon);
		}
		CCNode* rect = (CCNode*)tile->getChildren()->objectAtIndex(0);
		CCNode* icon = (CCNode*)tile->getChildren()->objectAtIndex(1);
		rect->setVisible(tileState != 0 && !flipped);
		icon->setVisible(flipped);
	}
}

int View::sceneOrder(CCNode* scene)
{
	if(scene == mpSceneConnect) return 1;
	else if(scene == mpSceneLobby) return 2;
	else if(scene == mpSceneGame) return 3;
	else return 0;
}

void View::switchScene(CCNode* scene)
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	float w = sz.width;
	if(mpCurrentScene != scene)
	{
		bool back = false;
		if(mpCurrentScene)
		{
			if(sceneOrder(scene) < sceneOrder(mpCurrentScene))
				back = true;
			if(back)
				mpCurrentScene->runAction(CCMoveTo::create(0.4f, ccp(w, mpCurrentScene->getPositionY())));
			else
				mpCurrentScene->runAction(CCMoveTo::create(0.4f, ccp(-w, mpCurrentScene->getPositionY())));
		}		
		if(back)
			scene->setPositionX(-w);
		else
			scene->setPositionX(w);
		scene->runAction(CCMoveTo::create(0.4f, ccp(0, scene->getPositionY())));

		mpCurrentScene = scene;
	}
}