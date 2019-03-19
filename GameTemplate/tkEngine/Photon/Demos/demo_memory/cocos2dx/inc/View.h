#pragma once

#include "cocos2d.h"
#include "../../src/LoadBalancingListener.h"
#include "../../src/BaseView.h"
#include "ListController.h"

class View : public cocos2d::CCLayer, public BaseView, public cocos2d::CCTextFieldDelegate
{
public:
	View(void);
	~View(void);

	static View* create(void);
	
	// BaseView
	virtual void onStateChange(int state, const ExitGames::Common::JString& stateStr, bool inLobby, bool inRoom);
	virtual void refreshStateInfo(const ExitGames::Common::JString& stateInfo);
	virtual void refreshMessage(const ExitGames::Common::JString& msg);

    virtual void setup(void);
    virtual void setupBoard(int tileCols, int tileRows);
	virtual void updateBoard(unsigned char* tiles, unsigned char flippedTiles[2]);
	virtual void updateRoomList(const ExitGames::Common::JVector<ExitGames::Common::JString>& roomNames);
	virtual void updateSavedRoomList(const ExitGames::Common::JVector<ExitGames::Common::JString>& savedRoomNames);

	//cocos2d::CCLayer
	virtual bool init(void);
	virtual void onExit(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void keyBackClicked(void);

	// cocos2d::CCTextFieldDelegate
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, const ExitGames::Common::JString& text, int nLen);


	// self
	void init(LoadBalancingListener* lbl);
private: 
	void setupUI(void);

	LoadBalancingListener* mpLbl;
	float mBoardSize;
	float mTileSize;
	float mTileMargin;
	int mTileRows;
	int mTileCols;

	void switchScene(cocos2d::CCNode*);
	int sceneOrder(cocos2d::CCNode*);
	const char* getInputUserName(void);

	// scenes as nodes let keep unmovable info overlay
	cocos2d::CCNode* mpSceneConnect;
	cocos2d::CCNode* mpSceneLobby;
	cocos2d::CCNode* mpSceneGame;

	cocos2d::CCNode* mpCurrentScene;

	// all scenes
	cocos2d::CCLabelBMFont* mpLabelState;
	cocos2d::CCLabelBMFont* mpLabelLog;
	void menuLeaveCallback(CCObject* pSender);
	ExitGames::Common::JString mStateString;

	// mpSceneConnect
#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
	cocos2d::CCLabelBMFont* mpInputUserName;
#else
	cocos2d::CCTextFieldTTF* mpInputUserName;
#endif
	void menuInputUserNameTouchCallback(CCObject* pSender);
	void menuConnectCallback(CCObject* pSender);

	// mpSceneLobby
	cocos2d::CCLabelBMFont* mpLobbyStats1;
	cocos2d::CCLabelBMFont* mpLobbyStats2;
	ListController* mpRoomList;
	ListController* mpSavedRoomList;
	void roomListCallback(const ExitGames::Common::JString& name);
	void savedRoomListCallback(const ExitGames::Common::JString& name);

	void menuNewGameCallback(CCObject* pSender);
	void menuRandomGameCallback(CCObject* pSender);
	void menuRefreshCallback(CCObject* pSender);	
	
	// mpSceneGame
	cocos2d::CCLabelBMFont* mpLabelMessage;
	cocos2d::CCDrawNode* mpBoard;
	cocos2d::CCNode* mpTiles;

	void menuSuspendCallback(CCObject* pSender);
	void menuAbandonCallback(CCObject* pSender);
};