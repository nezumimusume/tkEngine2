/*!
 *@brief	tkEngine用の各種構造体。
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	エンジン初期化用のパラメータ。
	 */
	struct SInitParam{
		SInitParam(){
			memset(this, 0, sizeof(SInitParam));
			graphicsConfing.shadowRenderConfig.Init();
			graphicsConfing.tonemapConfig.Init();
			gameObjectPrioMax = 32;
		}
		HINSTANCE hInstance;				//!<アプリケーションインスタンス。
		int	screenWidth;					//!<スクリーンの幅。
		int	screenHeight;					//!<スクリーンの高さ。
		int frameBufferHeight;				//!<フレームバッファの幅。これが内部解像度。
		int frameBufferWidth;				//!<フレームバッファの高さ。これが内部解像度。
		int screenWidth2D;					//!<2D空間の幅。内部解像度が下がってもこれを変更しなければ、レイアウトを変更する必要はない。
		int screenHeight2D;					//!<2D空間の高さ。内部解像度が下がってもこれを変更しなければ、レイアウトを変更する必要はない。
		int nCmdShow;						//!<
		unsigned char gameObjectPrioMax;	//!<ゲームオブジェクトの優先度の最大値(32まで)
		SGraphicsConfig graphicsConfing;	//!<描画コンフィグ。
	};
}