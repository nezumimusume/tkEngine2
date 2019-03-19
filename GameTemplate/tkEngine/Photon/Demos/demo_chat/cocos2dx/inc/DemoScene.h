#pragma once

#include "cocos2d.h"

#include "Chat-cpp/inc/Client.h"
#include "Chat-cpp/inc/Listener.h"
#include "ListController.h"

class DemoScene : public cocos2d::CCLayer, public cocos2d::CCTextFieldDelegate, public ExitGames::Chat::Listener
{
public:
	DemoScene(void);
	~DemoScene(void);
	static DemoScene* create(void);
	
	void setClient(ExitGames::Chat::Client* client);
	void setupUI(void);

	// ExitGames::Chat::Listener
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);
	virtual void onStateChange(int state);
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void disconnectReturn(void);
	virtual void subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels, const ExitGames::Common::JVector<bool>& results);
	virtual void unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels);
	virtual void onGetMessages(const ExitGames::Common::JString& channelName, const ExitGames::Common::JVector<ExitGames::Common::JString>& senders, const ExitGames::Common::JVector<ExitGames::Common::Object>& messages);
	virtual void onPrivateMessage(const ExitGames::Common::JString& sender, const ExitGames::Common::Object& message, const ExitGames::Common::JString& channelName);
	virtual void onStatusUpdate(const ExitGames::Common::JString& user, int status, bool gotMessage, const ExitGames::Common::Object& message);

	// cocos2d::CCLayer
	virtual bool init(void);
	// assert triggered if not overridden
	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

	// cocos2d::CCTextFieldDelegate
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, const char* text, int nLen);
	virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF* sender, const char* delText, int nLen);
	// Move scene to avoid text field overlap with keyboard. Should use CCIMEDelegate::keyboardWillShow instead but it does not work properly.
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF* sender);
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* sender);

private:
	ExitGames::Common::Logger mLogger;
	ExitGames::Chat::Client* mpClient;
	ExitGames::Common::JString mUserID;

	cocos2d::CCLabelTTF* mpLabelState;
	cocos2d::CCTextFieldTTF* mpUserIDInput;
	cocos2d::CCTextFieldTTF* mpMessageInput;
	// scroll support
	float messageInputMaxWidth;
	void messageInputScroll(void);

	// subscribe to channels or friends
	cocos2d::CCMenuItem* mppSubscribeButtons[2];
	cocos2d::CCMenuItem* mppFriendButtons[2];
	int subscribeButtonMode;
	void subscribeButtonModeSet(int);
	int mFriendButtonMode;
	void friendButtonModeSet(int);

	// button made of user id inout - attaches keyboard on press
	void menuUserIDTouchCallback(CCObject* pSender);
	// button made of message inout - attaches keyboard on press
	void menuMessageTouchCallback(CCObject* pSender);
	// other buttons
	void menuConnectCallback(CCObject* pSender);
	void menuDisconnectCallback(CCObject* pSender);
	void menuSubscribeCallback(CCObject* pSender);
	void menuSetFriendsCallback(CCObject* pSender);
	void menuClearFriendsCallback(CCObject* pSender);
	void menuUnsubscribeCallback(CCObject* pSender);
	void menuSendCallback(CCObject* pSender);
	// user status change buttons
	void menuUserStatusCallback(CCObject* pSender);

	void onTouch(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	// buttons mask the scene and prevent keyboard detach from input when touch is out of input, need to call this on every button press
	void resetInputFocus(void);

	// empty (not used)
	void logTouchCallback(const ExitGames::Common::JString& name);

	void sendMessage(void);
};