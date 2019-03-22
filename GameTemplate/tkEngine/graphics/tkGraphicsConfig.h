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
		float	lightHeight = UnitM(20.0f);		//!<ライトの高さ。
		float	depthOffset[NUM_SHADOW_MAP] = { 0.0002f , 0.0002f , 0.0002f };	//!<シャドウオフセット。この値を大きくすると、影が落ちにくくなる。
		float	offsetTexelWorld = 0.05f;			//!<ブラーの時にフェッチするオフセット座標。ワールド空間の量。値が大きいほど大きくボケる。
		EnSoftShadowQualityLevel	softShadowLevel = EnSoftShadowQualityLevel::enNone;		//!<ソフトシャドウの品質レベル。値が大きいほど品質が上がる。最大２
																							//0：ハードシャドウ、1：ScreenSpaceSoftShadow、2:ScreenSpaceSoftShadow + PCF
	};
	/*!
	* @brief	全方位シャドウ描画のコンフィグ。
	*/
	struct SOminiShadowRenderConfig {
		bool isEnable = false;		//全方位シャドウが有効かどうかのフラグ。
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
		SShadowRenderConfig				shadowRenderConfig;			//影の処理のコンフィグ。
		SOminiShadowRenderConfig		ominiShadowRenderConfig;	//全方位シャドウのコンフィグ。	
		SBloomConfig					bloomConfig;				//Bloomのコンフィグ。
		SDofConfig						dofConfig;					//被写界深度のコンフィグ。
		SAAConfig						aaConfig;					//アンチエイリアスのコンフィグ。
		SMotionBlurConfig				motionBlurConfig;			//モーションブラーのコンフィグ。
		STonemapConfig					tonemapConfig;			//トーンマップのコンフィグ。
		SDitheringConfig				ditheringConfig;		//ディザリング。
		SScreenSpaceReflectionConfig	ssrConfig;				//ssrコンフィグ
	};
}
