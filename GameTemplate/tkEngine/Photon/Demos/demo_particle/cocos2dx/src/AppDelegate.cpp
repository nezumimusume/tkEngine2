#include "cocos2d.h"
#include "AppDelegate.h"
#include "Console.h"
#include "DemoScene.h"
#include "ViewConstants.h"
#include "../../src/DemoConstants.h"
#include "../../src/LoadBalancingListener.h"

bool useTcp = false;

using namespace cocos2d;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::Common;
using namespace ExitGames::Photon;

#define LOGIC_TICK_INTERVAL 0.01f

class LogicTick : public CCNode
{
public:
	LogicTick(Client* pLbc, LoadBalancingListener* pLbl, DemoScene* pScene)
		: mpLbc(pLbc)
		, mpLbl(pLbl)
		, mpScene(pScene)
	{
		schedule(schedule_selector(LogicTick::tick), LOGIC_TICK_INTERVAL);
	}
	void tick(float)
	{
		mpLbc->service();
		mpLbl->service();
		mpScene->tick(LOGIC_TICK_INTERVAL);
	}
private:
	Client* mpLbc;
	LoadBalancingListener* mpLbl;
	DemoScene* mpScene;
};

AppDelegate::AppDelegate(void)
{
}

AppDelegate::~AppDelegate(void)
{
}

bool AppDelegate::applicationDidFinishLaunching(void)
{
	LoadBalancingListener* pLbl = NULL;
	Client* pLbc = NULL;

	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	if(screenSize.width < 1) // 0 for desktop - set to default values
		CCEGLView::sharedOpenGLView()->setFrameSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	else // set to device resolution
		CCEGLView::sharedOpenGLView()->setFrameSize(screenSize.width, screenSize.height);

	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	// sets landscape mode
#ifndef CC_PLATFORM_MARMALADE
	pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
#endif

	// turn on display FPS
//	pDirector->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	// create a pScene. it's an autorelease object
	CCScene* pRootScene = CCScene::create();
	DemoScene* pScene = DemoScene::create();
	pRootScene->addChild(pScene);

	pLbl = new LoadBalancingListener(pScene);
	pLbc = new Client(*pLbl, appID, appVersion, useTcp);
	pLbc->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	ExitGames::Common::Base::setListener(pLbl);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
//	pLbc->setCRCEnabled(true);
	pLbl->setLBC(pLbc);
	pLbc->setTrafficStatsEnabled(true);
//	pLbc->setLimitOfUnreliableCommands(15);
	pScene->setLBC(pLbc, pLbl);
	pScene->setupUI();

	Console::get().writeLine(L"LBC: connecting...");
	pLbl->connect(JString(L"NV")+GETTIMEMS());
	LogicTick* logicTick = new LogicTick(pLbc, pLbl, pScene);
	pRootScene->addChild(logicTick);
	// run
	pDirector->runWithScene(pRootScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground(void)
{
	CCDirector::sharedDirector()->pause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground(void)
{
	CCDirector::sharedDirector()->resume();
}