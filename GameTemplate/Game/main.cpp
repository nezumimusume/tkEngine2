/*!
 *@brief	main.cpp
 */
#include "stdafx.h"
#include "Game.h"

namespace {
	/*!
	*@brief	低スペックPC向けにtkEngineのConfigを設定する。
	*@param[out]	initParam	tkEngineの初期化パラメータ
	*/
	void SetTkEngineConfigForLowSpec(SInitParam& initParam)
	{
		initParam.screenWidth = 1280;
		initParam.screenHeight = 720;
		initParam.frameBufferWidth = 640;
		initParam.frameBufferHeight = 360;
		//影の設定。
		initParam.graphicsConfing.shadowRenderConfig.isEnable = true;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapWidth = 512;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapHeight = 512;
		initParam.graphicsConfing.shadowRenderConfig.lightHeight = UnitM(20.0f);
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[0] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[1] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[2] = 0.002f;
		initParam.graphicsConfing.shadowRenderConfig.softShadowLevel = EnSoftShadowQualityLevel::eSSSS_PCF;

		//アンチ
		initParam.graphicsConfing.aaConfig.isEnable = false;
		//Bloom
		initParam.graphicsConfing.bloomConfig.isEnable = true;
		//tonemap
		initParam.graphicsConfing.tonemapConfig.isEnable = true;
		//SSR
		initParam.graphicsConfing.ssrConfig.isEnable = false;
		//ディザ
		initParam.graphicsConfing.ditheringConfig.isEnable = false;
		//dof
		initParam.graphicsConfing.dofConfig.isEnable = false;
	}
	/*!
	*@brief	中スペックPC向けにtkEngineのConfigを設定する。
	*@param[out]	initParam	tkEngineの初期化パラメータ
	*/
	void SetTkEngineConfigForMiddleSpec(SInitParam& initParam)
	{
		initParam.screenWidth = 1280;
		initParam.screenHeight = 720;
		initParam.frameBufferWidth = 1280;
		initParam.frameBufferHeight = 720;
		
		//影の設定。
		initParam.graphicsConfing.shadowRenderConfig.isEnable = true;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapWidth = 1024;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapHeight = 1024;
		initParam.graphicsConfing.shadowRenderConfig.lightHeight = UnitM(20.0f);
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[0] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[1] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[2] = 0.002f;
		initParam.graphicsConfing.shadowRenderConfig.softShadowLevel = EnSoftShadowQualityLevel::eSSSS_PCF;

		//アンチ
		initParam.graphicsConfing.aaConfig.isEnable = false;
		//Bloom
		initParam.graphicsConfing.bloomConfig.isEnable = true;
		//tonemap
		initParam.graphicsConfing.tonemapConfig.isEnable = true;
		initParam.graphicsConfing.tonemapConfig.luminance = 0.56f;

		//SSR
		initParam.graphicsConfing.ssrConfig.isEnable = true;
		//ディザ
		initParam.graphicsConfing.ditheringConfig.isEnable = false;
		//dof
		initParam.graphicsConfing.dofConfig.isEnable = true;
	}
	/*!
	*@brief	高スペックPC向けにtkEngineのConfigを設定する。
	*@param[out]	initParam	tkEngineの初期化パラメータ
	*/
	void SetTkEngineConfigForHighSpec(SInitParam& initParam)
	{
		initParam.screenWidth = 1920;
		initParam.screenHeight = 1080;
		initParam.frameBufferWidth = 1920;
		initParam.frameBufferHeight = 1080;
		//影の設定。
		initParam.graphicsConfing.shadowRenderConfig.isEnable = true;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapWidth = 2048;
		initParam.graphicsConfing.shadowRenderConfig.shadowMapHeight = 2048;
		initParam.graphicsConfing.shadowRenderConfig.lightHeight = UnitM(10.0f);
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[0] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[1] = 0.001f;
		initParam.graphicsConfing.shadowRenderConfig.depthOffset[2] = 0.002f;
		initParam.graphicsConfing.shadowRenderConfig.softShadowLevel = EnSoftShadowQualityLevel::eSSSS_PCF;

		//アンチ
		initParam.graphicsConfing.aaConfig.isEnable = true;
		//Bloom
		initParam.graphicsConfing.bloomConfig.isEnable = true;
		//tonemap
		initParam.graphicsConfing.tonemapConfig.isEnable = true;
		initParam.graphicsConfing.tonemapConfig.luminance = 0.56f;
		//SSR
		initParam.graphicsConfing.ssrConfig.isEnable = true;
		//ディザ
		initParam.graphicsConfing.ditheringConfig.isEnable = true;
		//dof
		initParam.graphicsConfing.dofConfig.isEnable = true;
	}
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned)time(NULL));
	//tkEngine2の初期化パラメータを設定する。
	SInitParam initParam;
	initParam.nCmdShow = nCmdShow;
	initParam.hInstance = hInstance;
#if defined(USE_LOW_SPEC_SETTINGS)
	//LowSpec向けにtkEngineのConfigを設定する。
	SetTkEngineConfigForLowSpec( initParam );
	
#elif defined(USE_MIDDLE_SPEC_SETTINGS)
	SetTkEngineConfigForMiddleSpec(initParam);
#else
	//HighSpec向けにtkEngineのConfigを設定する。
	SetTkEngineConfigForHighSpec(initParam);
	
#endif
	GraphicsEngine().GetShadowMap().SetFar(1000.0f);
	GraphicsEngine().GetShadowMap().SetNear(50.0f);

	//エンジンを初期化。
	if (Engine().Init(initParam) == true) {
		NewGO<Game>(0, nullptr);
		//ゲームループを実行。
		Engine().RunGameLoop();
	}
	//エンジンの終了処理。
	Engine().Final();

	return 0;
}

