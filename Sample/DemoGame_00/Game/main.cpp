/*!
 *@brief	main.cpp
 */
#include "stdafx.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned)time(NULL));
	//tkEngine2の初期化パラメータを設定する。
	SInitParam initParam;
	initParam.nCmdShow = nCmdShow;
	initParam.hInstance = hInstance;
	initParam.screenWidth = 1280;
	initParam.screenHeight = 720;
	initParam.frameBufferWidth = 1280;
	initParam.frameBufferHeight = 720;
	//影の設定。
	initParam.graphicsConfing.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfing.shadowRenderConfig.shadowMapWidth = 2048;
	initParam.graphicsConfing.shadowRenderConfig.shadowMapHeight = 2048;
	initParam.graphicsConfing.shadowRenderConfig.lightHeight = UnitM(20.0f);
	initParam.graphicsConfing.shadowRenderConfig.depthOffset[0] = 0.001f;
	initParam.graphicsConfing.shadowRenderConfig.depthOffset[1] = 0.001f;
	initParam.graphicsConfing.shadowRenderConfig.depthOffset[2] = 0.002f;
	initParam.graphicsConfing.shadowRenderConfig.softShadowLevel = EnSoftShadowQualityLevel::enNone;
	//アンチ
	initParam.graphicsConfing.aaConfig.isEnable = true;
	//Bloom
	initParam.graphicsConfing.bloomConfig.isEnable = true;
	//tonemap
	initParam.graphicsConfing.tonemapConfig.isEnable = true;
	initParam.graphicsConfing.tonemapConfig.luminance = 0.34f;
	//ディザリング。
	initParam.graphicsConfing.ditheringConfig.isEnable = true;

	MainCamera().SetNear(100.0f);
	MainCamera().SetFar(10000.0f);
	
	//エンジンを初期化。
	if (Engine().Init(initParam) == true) {
		//SetOutputDebugVectorNameList({ "playerMoveSpeed" });
		SoundEngine().GetMasteringVoice()->SetVolume(0.1f);
		//NewGO<Game>(0, "Game");
		NewGO<Fade>(1, "Fade");
		NewGO<Title>(0, nullptr);
		//ゲームループを実行。
		Engine().RunGameLoop();
	}
	//エンジンの終了処理。
	Engine().Final();

	return 0;
}

