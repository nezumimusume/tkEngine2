#include "Console.h"
#include "DemoScene.h"
#include "../inc/ViewConstants.h"
#include "../../src/DemoConstants.h"

#ifdef _EG_ANDROID_PLATFORM
#include <android/log.h>
#endif

using namespace cocos2d;
using namespace ExitGames::Common;
using namespace ExitGames::Chat;

ListController* gpLogList = NULL;

class ConsoleImplementation
{
public:
	void write(const JString& str)
	{
		gpLogList->addItem(str);
		gpLogList->scrollToBottom();
	}

	void writeLine(const JString& str)
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

void Console::write(const JString& str)
{
	mpImp->write(str);
}

void Console::writeLine(const JString& str)
{
	mpImp->writeLine(str);
}

Console& Console::get(void)
{
	static Console console;
	return console;
}

// set according to screen size in DemoScene::init()
float FONT_SIZE = 32;

// index = const in ExitGames::Chat::UserStatus
static const JString UserStatusStr[] = 
{
	L"Offline",
	L"Invisible",
	L"Online",
	L"Away",
	L"Dnd",
	L"Lfg",
	L"Playing"
};

// index = const in ExitGames::Chat::ClientState::ClientState
static const JString ClientStateStr[] = 
{
	L"Uninitialized",
	L"ConnectingToNameServer",
	L"ConnectedToNameServer",
	L"Authenticating",
	L"Authenticated",
	L"DisconnectingFromNameServer",
	L"ConnectingToFrontEnd",
	L"ConnectedToFrontEnd",
	L"Disconnecting",
	L"Disconnected"
};

DemoScene::DemoScene(void)
	: mUserID(DEFAULT_USER_ID)
	, mpLabelState(NULL)
	, mpUserIDInput(NULL)
	, mpMessageInput(NULL)
	, messageInputMaxWidth(0)
	, subscribeButtonMode(0)
	, mFriendButtonMode(0)
{
	mppSubscribeButtons[0] = NULL;
	mppSubscribeButtons[1] = NULL;
	mppFriendButtons[0] = NULL;
	mppFriendButtons[1] = NULL;
}

DemoScene::~DemoScene(void)
{
}

DemoScene* DemoScene::create(void)
{
	DemoScene *pRet = new DemoScene();
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

// on "init" you need to initialize your instance
bool DemoScene::init(void)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	}
	while(0);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	float h = CCDirector::sharedDirector()->getWinSize().height;
	FONT_SIZE = h * 5 / 100;

	return bRet;
}

bool DemoScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return false;
}

void DemoScene::resetInputFocus(void)
{
	mpUserIDInput->detachWithIME();
	mpMessageInput->detachWithIME();
}

static void drawBorder(CCNode* node)
{
	CCDrawNode* d = CCDrawNode::create();
	float width = node->getContentSize().width;
	float height = node->getContentSize().height;
	float marginX = CCDirector::sharedDirector()->getWinSize().width / 100 + 2;
	float marginY = - height / 10 + 2; // negative for text labels
	CCPoint pp[] = {ccp(-marginX, -marginY), ccp(width + marginX, -marginY), ccp(width + marginX, height + marginY), ccp(-marginX, height + marginY)};
	d->drawPolygon(pp, 4, BUTTON_COLOR, 1, BUTTON_BORDER_COLOR);
	d->setAnchorPoint(node->getAnchorPoint());
	node->addChild(d);
}

void DemoScene::setupUI(void)
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	float y = sz.height;
	float spacing = sz.width / 10;
	ccColor3B grey = {180,180,180};

	CCLabelTTF* header = CCLabelTTF::create("Photon Chat Demo", FONT_NAME, FONT_SIZE * 0.7f);
	header->setColor(grey);
	header->setAnchorPoint(ccp(0, 1));
	header->setPosition(ccp(0, y));
	addChild(header);

	y -= header->getContentSize().height;

	mpLabelState = CCLabelTTF::create("Undef", FONT_NAME, FONT_SIZE * 0.7f);
	mpLabelState->setColor(grey);
	mpLabelState->setAnchorPoint(ccp(0, 1));
	mpLabelState->setPosition(ccp(0, y));
	addChild(mpLabelState);

	y -= mpLabelState->getContentSize().height;

	CCLabelTTF* userIDLabel = CCLabelTTF::create("User id: ", FONT_NAME, FONT_SIZE);
	userIDLabel->setAnchorPoint(ccp(0, 1));
	userIDLabel->setPosition(ccp(0, y));

	mpUserIDInput = CCTextFieldTTF::textFieldWithPlaceHolder("<set user id>", FONT_NAME, FONT_SIZE);
	mpUserIDInput->setDelegate(this);
	mpUserIDInput->setString(DEFAULT_USER_ID.ANSIRepresentation().cstr());
	mpUserIDInput->setColor(INPUT_COLOR);
	mpUserIDInput->setContentSize(CCSize(sz.width - userIDLabel->getContentSize().width, mpUserIDInput->getContentSize().height));
	CCMenuItemLabel* userIDInputButton = CCMenuItemLabel::create(mpUserIDInput, this, menu_selector(DemoScene::menuUserIDTouchCallback));
	userIDInputButton->setAnchorPoint(ccp(1, 1));
	CCMenu* userIDInputMenu = CCMenu::create(userIDInputButton, NULL);
	userIDInputMenu->setPosition(sz.width, y);

	addChild(userIDLabel);
	addChild(userIDInputMenu);

	y -= mpUserIDInput->getContentSize().height;

	CCMenuItem* b0 = CCMenuItemLabel::create(CCLabelTTF::create("Connect", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuConnectCallback));	
	CCMenuItem* b1 = CCMenuItemLabel::create(CCLabelTTF::create("Disconnect", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuDisconnectCallback));
	CCMenuItem* b20 = CCMenuItemLabel::create(CCLabelTTF::create("Subscribe", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuSubscribeCallback));
	CCMenuItem* b21 = CCMenuItemLabel::create(CCLabelTTF::create("Unsubscr.", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuUnsubscribeCallback));
	CCMenuItem* b30 = CCMenuItemLabel::create(CCLabelTTF::create("Set friends", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuSetFriendsCallback));
	CCMenuItem* b31 = CCMenuItemLabel::create(CCLabelTTF::create("Clr. friends", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuClearFriendsCallback));
	drawBorder(b0);
	drawBorder(b1);
	drawBorder(b20);
	drawBorder(b21);
	drawBorder(b30);
	drawBorder(b31);

	mppSubscribeButtons[0] = b20;
	mppSubscribeButtons[1] = b21;
	mppFriendButtons[0] = b30;
	mppFriendButtons[1] = b31;

	b0->setAnchorPoint(ccp(0, 1));
	b1->setAnchorPoint(ccp(1, 1));
	b20->setAnchorPoint(ccp(0, 1));
	b21->setAnchorPoint(ccp(0, 1));
	b30->setAnchorPoint(ccp(1, 1));
	b31->setAnchorPoint(ccp(1, 1));
	int bh = (int)b0->getContentSize().height;
	b0->setPosition(ccp(0, 0));
	b1->setPosition(ccp(sz.width, 0));
	b20->setPosition(ccp(0, -bh));
	b21->setPosition(ccp(0, -bh));
	b30->setPosition(ccp(sz.width, -bh));
	b31->setPosition(ccp(sz.width, -bh));

	CCMenu* pMenu = CCMenu::create(b0, b1, b20, b21, b30, b31, NULL);
	pMenu->setPosition(0, y);
	addChild(pMenu);

	y -= bh*2;
	
	const float logHeight = sz.height/3;
	gpLogList = ListController::create(sz.width, logHeight, this, (ListControllerHandler)&DemoScene::logTouchCallback);
	gpLogList->setAnchorPoint(ccp(0, 1));
	gpLogList->setPosition(ccp(0, y));
	addChild(gpLogList);

	y -= logHeight;

	y -= FONT_SIZE/2;

	CCMenuItem* sendButton = CCMenuItemLabel::create(CCLabelTTF::create("Send", FONT_NAME, FONT_SIZE * 1.2f), this, menu_selector(DemoScene::menuSendCallback));
	drawBorder(sendButton);
	sendButton->setAnchorPoint(ccp(1, 1));
	sendButton->setPosition(ccp(sz.width, 0));
	CCMenu* sendMenu = CCMenu::create(sendButton, NULL);
	sendMenu->setPosition(0, y);
	addChild(sendMenu);

	messageInputMaxWidth = sz.width - sendButton->getContentSize().width - spacing;
	mpMessageInput = CCTextFieldTTF::textFieldWithPlaceHolder("<message>", CCSize(0, 0), kCCTextAlignmentLeft, FONT_NAME, FONT_SIZE * 1.2f);
	mpMessageInput->setDelegate(this);
	mpMessageInput->setColor(INPUT_COLOR);
	CCMenuItemLabel* messageInputButton = CCMenuItemLabel::create(mpMessageInput, this, menu_selector(DemoScene::menuMessageTouchCallback));
	messageInputButton->setAnchorPoint(ccp(0, 1));
	CCMenu* messageInputMenu = CCMenu::create(messageInputButton, NULL);
	messageInputMenu->setPosition(0, y);
	addChild(messageInputMenu);

	y -= mpMessageInput->getContentSize().height;

	y -= FONT_SIZE/2;

	CCMenu* userStatusMenu = CCMenu::create();
	userStatusMenu->setPosition(0, 0);
	float x = 0;
	for(int i=0; i<sizeof(UserStatusStr)/sizeof(JString); ++i)
	{
		CCLabelTTF* l = CCLabelTTF::create(UserStatusStr[i].UTF8Representation().cstr(), FONT_NAME, FONT_SIZE);
		drawBorder(l);
		CCMenuItem* b = CCMenuItemLabel::create(l, this, menu_selector(DemoScene::menuUserStatusCallback));
		b->setTag(i);
		b->setAnchorPoint(ccp(0, 1));
		CCSize size = b->getContentSize();
		if(x + size.width > sz.width)
		{
			x = 0;
			y -= size.height;
		}
		b->setPosition(ccp(x, y));
		x += size.width + spacing;
		userStatusMenu->addChild(b);
	}
	addChild(userStatusMenu);

	subscribeButtonModeSet(0);
	friendButtonModeSet(0);
}

void DemoScene::subscribeButtonModeSet(int m)
{
	subscribeButtonMode = m;
	if(m >= 0 && m < 2)
	{
		mppSubscribeButtons[m]->setVisible(true);
		mppSubscribeButtons[1 - m]->setVisible(false);
	}
}

void DemoScene::friendButtonModeSet(int m)
{
	mFriendButtonMode = m;
	if(m >= 0 && m < 2)
	{
		mppFriendButtons[m]->setVisible(true);
		mppFriendButtons[1 - m]->setVisible(false);
	}
}

void DemoScene::messageInputScroll(void)
{
	float pos = messageInputMaxWidth - mpMessageInput->getContentSize().width;
	if(pos < 0) 
		mpMessageInput->setPositionX(pos);
	else
		mpMessageInput->setPositionX(0);
}

bool DemoScene::onTextFieldInsertText(cocos2d::CCTextFieldTTF* sender, const char* text, int nLen)
{
	if(sender == mpMessageInput)
		messageInputScroll();
	if(sender == mpMessageInput && '\n' == *text) 
		sendMessage();
	return false;
}

bool DemoScene::onTextFieldDeleteBackward(CCTextFieldTTF* sender, const char* delText, int nLen)
{
	if(sender == mpMessageInput)
		messageInputScroll();
	return false;
}

bool DemoScene::onTextFieldAttachWithIME(CCTextFieldTTF* sender)
{
#ifndef _EG_WINDOWS_PLATFORM
#ifndef _EG_MARMALADE_PLATFORM
	float y = sender->convertToWorldSpace(ccp(0,0)).y;
	float y0 = CCDirector::sharedDirector()->getWinSize().height / 2;
	if(y < y0)
		setPositionY(y0 - y);
#endif
#endif
	return false;
}

bool DemoScene::onTextFieldDetachWithIME(CCTextFieldTTF* sender)
{
#ifndef _EG_WINDOWS_PLATFORM
#ifndef _EG_MARMALADE_PLATFORM
	setPositionY(0);
#endif
#endif
	return false;
}

void DemoScene::setClient(Client* c)
{
	this->mpClient = c;
}

void DemoScene::menuUserIDTouchCallback(CCObject* pSender)
{
	mpUserIDInput->attachWithIME();
}

void DemoScene::menuMessageTouchCallback(CCObject* pSender)
{
	mpMessageInput->attachWithIME();
}

void DemoScene::menuConnectCallback(CCObject* pSender)
{
	resetInputFocus();

	Console::get().writeLine(L"[i]: Connecting...");
	mUserID = mpUserIDInput->getString();
	mpClient->connect(AuthenticationValues().setUserID(mUserID));
}

void DemoScene::menuDisconnectCallback(CCObject* pSender)
{
	resetInputFocus();

	if(mpClient->getState() == ClientState::Uninitialized || mpClient->getState() == ClientState::Disconnected)
		CCDirector::sharedDirector()->popScene();
	else
		mpClient->disconnect();
}

void DemoScene::menuSubscribeCallback(CCObject* pSender)
{
	resetInputFocus();

	int size = sizeof(SUBSCRIBE_CHANNELS)/sizeof(JString);
	JVector<JString> chs(size);
	for(int i=0; i<size; ++i)
		chs.addElement(SUBSCRIBE_CHANNELS[i]);
	if(mpClient->opSubscribe(chs))
		Console::get().writeLine(L"[i]: Subscribing...");
	else
		Console::get().writeLine(L"[i]: Connect first");
}

void DemoScene::menuSetFriendsCallback(CCObject* pSender)
{
	resetInputFocus();

	JVector<JString> friends(USER_COUNT);
	for(int i=0; i<USER_COUNT; ++i)
		friends.addElement(USER_PREFIX+i);
	if(mpClient->opAddFriends(friends))
	{
		Console::get().writeLine(L"[i]: Setting friends: " + friends.toString() + L" ...");
		friendButtonModeSet(1);
	}
	else
		Console::get().writeLine(L"[i]: Connect first");
}

void DemoScene::menuClearFriendsCallback(CCObject* pSender)
{
	resetInputFocus();

	JVector<JString> friends(USER_COUNT);
	for(int i=0; i<USER_COUNT; ++i)
		friends.addElement(USER_PREFIX+i);
	if(mpClient->opRemoveFriends(friends))
		Console::get().writeLine(L"[i]: Clearing friends...");
	else
		Console::get().writeLine(L"[i]: Connect first");
	friendButtonModeSet(0);
}

void DemoScene::menuUnsubscribeCallback(CCObject* pSender)
{
	resetInputFocus();

	int size = sizeof(SUBSCRIBE_CHANNELS)/sizeof(JString);
	JVector<JString> chs(size);
	for(int i=0; i<size; ++i)
		chs.addElement(SUBSCRIBE_CHANNELS[i]);
	if(mpClient->opUnsubscribe(chs))
		Console::get().writeLine(L"[i]: Unsubscribing...");
	else
		Console::get().writeLine(L"[i]: Connect first");
}

void DemoScene::menuSendCallback(CCObject* pSender) 
{
	sendMessage();
}

void DemoScene::menuUserStatusCallback(CCObject* pSender)
{
	resetInputFocus();

	CCMenuItemLabel* item = (CCMenuItemLabel*)pSender;
	if(mpClient->opSetOnlineStatus(item->getTag(), L"My status changed")) // update message
		Console::get().writeLine(JString(L"[i] my status sent: ") + item->getTag());
}

void DemoScene::sendMessage(void) 
{
	JString s = mpMessageInput->getString();
	int index;
	JString sep = s.substring((index=s.indexOf(L':'))<0?index:index+1);
	if(sep.length()) // publish
		mpClient->opPublishMessage(s.substring(0, index), sep);
	else if((sep=s.substring((index=s.indexOf(L'@'))<0?index:index+1)).length()) // private
			mpClient->opSendPrivateMessage(s.substring(0, index), sep);
	else // publish to random channel
	{
		static int cnt = 0;
		const JVector<Channel*>& chs = mpClient->getPublicChannels();
		if(chs.getSize())
			mpClient->opPublishMessage(chs[cnt++ % chs.getSize()]->getName(), s);
		else
			Console::get().writeLine(L"[i]: Not subscribed to any channels");
	}
	mpMessageInput->setString(NULL);
	messageInputScroll();
}

void DemoScene::logTouchCallback(const JString& name)
{
}

void DemoScene::debugReturn(int debugLevel, const JString& string)
{
	Console::get().debugReturn(debugLevel, string);
}

void DemoScene::onStateChange(int state)
{
	mpLabelState->setString((JString()+(state >=0 && state < sizeof(ClientStateStr)/sizeof(JString) ? ClientStateStr[state] : "?")).UTF8Representation().cstr());
	if(state == ClientState::ConnectedToFrontEnd)
	{
		Console::get().writeLine(L"[i]: [Subscribe] for public channels");
		Console::get().writeLine(L"[i]:    or type in 'userid@message'");
		Console::get().writeLine(L"[i]:    and press 'OK' for private");
		subscribeButtonModeSet(0);
	}
	else if(state == ClientState::Uninitialized || state == ClientState::Disconnected)
	{
		if(mpClient->getDisconnectedCause() == DisconnectCause::INVALID_AUTHENTICATION)
		{
			Console::get().writeLine(L"[i]: Disconnected due to invalid authentication");
			Console::get().writeLine(L"[i]: Is app id correct?");
		}
		else
			Console::get().writeLine(L"[i]: Disconnected");
		Console::get().writeLine(L"-------------------------------------------------");
		Console::get().writeLine(L"[i]: touch log lines to see details");
		Console::get().writeLine(L"[i]: type in user id and press [Connect]");
		subscribeButtonModeSet(0);
		friendButtonModeSet(0);
	}
}

void DemoScene::connectionErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"connection failed with error %d", errorCode);
	Console::get().writeLine(JString(L"[i]: Connection Error ") + errorCode);
}

void DemoScene::clientErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"received error %d from client", errorCode);
	Console::get().writeLine(JString(L"[i]: Error ") + errorCode);
}

void DemoScene::warningReturn(int warningCode)
{
	EGLOG(DebugLevel::WARNINGS, L"received warning %d from client", warningCode);
	Console::get().writeLine(JString(L"[i]: Warning ") + warningCode);
}

void DemoScene::serverErrorReturn(int errorCode)
{
	EGLOG(DebugLevel::ERRORS, L"received error %d from server", errorCode);
	Console::get().writeLine(JString(L"[i]: Server Error ") + errorCode);
}

void DemoScene::connectReturn(int errorCode, const JString& errorString)
{
	Console::get().writeLine(L"[i]: Connected to Front End");
}

void DemoScene::disconnectReturn(void)
{	
}

void DemoScene::subscribeReturn(const JVector<JString>& channels, const JVector<bool>& results)
{
	JVector<JString> ok(100);
	for(unsigned int i=0; i<channels.getSize(); ++i) 
		if(results[i])
			ok.addElement(channels[i]);
	Console::get().writeLine(L"[i]: Subscribed to " + ok.toString());
	Console::get().writeLine(L"[i]: type in 'channel:message'");
	Console::get().writeLine(L"[i]:   and press [Send] to publish");
	subscribeButtonModeSet(1);
}

void DemoScene::unsubscribeReturn(const JVector<JString>& channels)
{
	Console::get().writeLine(L"[i]: Unsubscribed from " + channels.toString());
	subscribeButtonModeSet(0);
}

void DemoScene::onGetMessages(const JString& channelName, const JVector<JString>& senders, const JVector<Object>& messages)
{
	for(unsigned int i=0; i<senders.getSize(); ++i) 
		Console::get().writeLine(L"["+ channelName + L":"+ senders[i] + L"]: " + messages[i].toString());
}

void DemoScene::onPrivateMessage(const JString& sender, const Object& message, const JString& channelName)
{
	Console::get().writeLine(L"["+ channelName + L"@"+ sender + L"]: " + message.toString());
}

void DemoScene::onStatusUpdate(const JString& user, int status, bool gotMessage, const Object& message)
{
	const JString statusStr = (status < sizeof(UserStatusStr)/sizeof(JString)) ?  UserStatusStr[status] : L"";
	Console::get().writeLine(L"[i]: " + user + L": " + statusStr + L"(" + status + L") / " + (gotMessage?message.toString():L"[message skipped]"));
}
