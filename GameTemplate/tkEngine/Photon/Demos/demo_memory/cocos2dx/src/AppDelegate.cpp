#include "cocos2d.h"
#include "AppDelegate.h"
#include "View.h"
#include "ViewConstants.h"
#include "../../src/LoadBalancingListener.h"
#include "../../src/Game.h"
#include "../../src/DemoConstants.h"

using namespace cocos2d;
#define LOGIC_TICK_INTERVAL 0.01f

class LogicTick : public CCNode
{
public:
	LogicTick(ExitGames::LoadBalancing::Client* pLbc, LoadBalancingListener* pLbl)
		: mpLbc(pLbc)
		, mpLbl(pLbl)
	{
		schedule(schedule_selector(LogicTick::tick), LOGIC_TICK_INTERVAL);
	}
	void tick(float)
	{
		mpLbc->service();
		mpLbl->service();
	}
private:
	ExitGames::LoadBalancing::Client* mpLbc;
	LoadBalancingListener* mpLbl;
};

AppDelegate::AppDelegate(void)
{
}

AppDelegate::~AppDelegate(void)
{
}

bool AppDelegate::applicationDidFinishLaunching(void)
{
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	if(screenSize.width < 1) // 0 for desktop - set to default values
		CCEGLView::sharedOpenGLView()->setFrameSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	else // set to device resolution
		CCEGLView::sharedOpenGLView()->setFrameSize(screenSize.width, screenSize.height);

	// initialize director
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	// sets landscape mode
#ifndef CC_PLATFORM_MARMALADE
	pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
#endif

	// turn on display FPS
//	pDirector->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	CCScene* pRootScene = CCScene::create();
	View* pView = View::create();
	pRootScene->addChild(pView);

	LoadBalancingListener* pLbl = new LoadBalancingListener();
	ExitGames::LoadBalancing::Client* pLbc = new ExitGames::LoadBalancing::Client(*pLbl, APP_ID, APP_VERSION);
	pLbc->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	ExitGames::Common::Base::setListener(pLbl);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	pView->init(pLbl);
	pView->setup();
	pLbl->init(pLbc, pView);


	LogicTick* pLogicTick = new LogicTick(pLbc, pLbl);;
	pRootScene->addChild(pLogicTick);
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