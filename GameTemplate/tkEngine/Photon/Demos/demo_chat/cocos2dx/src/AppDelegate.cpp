#include "cocos2d.h"

#include "Chat-cpp/inc/Listener.h"
#include "../../src/DemoConstants.h"
#include "../inc/AppDelegate.h"
#include "../inc/DemoScene.h"
#include "../inc/ViewConstants.h"

bool useTcp = false;

using namespace cocos2d;
using namespace ExitGames::Chat;
using namespace ExitGames::Common;
using namespace ExitGames::Photon;

#define SERVICE_TICK_INTERVAL 0.01f

class ServiceTick : public CCNode
{
public:
	ServiceTick(Client* pClient)
		: pClient(pClient)
	{
		schedule(schedule_selector(ServiceTick::tick), SERVICE_TICK_INTERVAL);
	}
	void tick(float)
	{
		pClient->service();
	}
private:
	Client* pClient;
};

AppDelegate::AppDelegate(void)
{
}

AppDelegate::~AppDelegate(void)
{
}

bool AppDelegate::applicationDidFinishLaunching(void)
{
	Client* pClient = NULL;

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

	pClient = new Client(*pScene, APP_ID, APP_VERSION, ConnectionProtocol::UDP);
	pClient->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	ExitGames::Common::Base::setListener(pScene);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	pScene->setClient(pClient);
	pScene->setupUI();

	ServiceTick* serviceTick = new ServiceTick(pClient);
	pRootScene->addChild(serviceTick);
	// run
	pDirector->runWithScene(pRootScene);

	// trigger start hint
	pScene->onStateChange(pClient->getState());

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