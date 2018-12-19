/*!
 * @brief	グラフィックスコンフィグ。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	影処理のコンフィグ。
	 */
	struct SShadowRenderConfig{
		bool 	isEnable = false;				//!<影を落とす処理が有効かどうかのフラグ。
		int		shadowMapWidth = 512;			//!<シャドウマップの幅。
		int		shadowMapHeight = 512;			//!<シャドウマップの高さ。
		float	nearPlane = -1.0f;				//!<近平面までの距離。(設定を行わなければ自動計算されます。)				
		float	farPlane = -1.0f;				//!<遠平面までの距離。(設定を行わなければ自動計算されます。)				
		float	lightHeight = UnitM(20.0f);		//!<ライトの高さ。
		float	depthOffset[NUM_SHADOW_MAP] = { 0.0002f , 0.0002f , 0.0002f };	//!<シャドウオフセット。この値を大きくすると、影が落ちにくくなる。
		float	offsetTexelWorld = 0.05f;			//!<ブラーの時にフェッチするオフセット座標。ワールド空間の量。値が大きいほど大きくボケる。
		EnSoftShadowQualityLevel	softShadowLevel = EnSoftShadowQualityLevel::enNone;		//!<ソフトシャドウの品質レベル。値が大きいほど品質が上がる。最大２
																							//0：ハードシャドウ、1：ScreenSpaceSoftShadow、2:ScreenSpaceSoftShadow + PCF
	};
	/*!
	* @brief	Bloomのコンフィグ
	*/
	struct SBloomConfig {		
		bool	isEnable = false;		//!<ブルームが有効かどうかのフラグ。
	};

	/*!
	* @brief	被写界深度のコンフィグ。
	*/
	struct SDofConfig {
		bool isEnable = false;					//被写界深度が有効かどうかのフラグ。
	};
	/*!
	* @brief	アンチエイリアスのコンフィグ。
	*/
	struct SAAConfig {
		bool isEnable = false;					//アンチエイリアスが有効かどうかのフラグ。
	};
	/*!
	* @brief	モーションブラーのコンフィグ。
	*/
	struct SMotionBlurConfig {
		bool isEnable = false;
	};
	/*!
	* @brief	トーンマップのコンフィグ。
	*/
	struct STonemapConfig {

		bool isEnable = false;
		float luminance = 0.34f;			//明るさ。値が大きいほど明るくなります。
	};
	/*!
	* @brief	ディザリングのコンフィグ。
	*/
	struct SDitheringConfig {
		bool isEnable = false;
	};
	/*!
	* @brief	SSRのコンフィグ。
	*/
	struct SScreenSpaceReflectionConfig {
		bool isEnable = false;
	};

	/*!
	 * @brief	グラフィックスコンフィグ。
	 */
	struct SGraphicsConfig{
		SShadowRenderConfig				shadowRenderConfig;		//影の処理のコンフィグ。
		SBloomConfig					bloomConfig;			//Bloomのコンフィグ。
		SDofConfig						dofConfig;				//被写界深度のコンフィグ。
		SAAConfig						aaConfig;				//アンチエイリアスのコンフィグ。
		SMotionBlurConfig				motionBlurConfig;		//モーションブラーのコンフィグ。
		STonemapConfig					tonemapConfig;			//トーンマップのコンフィグ。
		SDitheringConfig				ditheringConfig;		//ディザリング。
		SScreenSpaceReflectionConfig	ssrConfig;				//ssrコンフィグ
	};
}
