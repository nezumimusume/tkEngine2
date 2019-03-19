// Application main file.

#include "../inc/AppDelegate.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#else

int main()
{

#endif

	AppDelegate app;

//#if(CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE) // Run() is used in original cocos sources for marmalade
	return cocos2d::CCApplication::sharedApplication()->run();
}