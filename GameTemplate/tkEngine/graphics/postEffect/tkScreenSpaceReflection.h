/*!
 *@brief	Screen Space Reflection
 */

#pragma once

#include <random>
#include "tkEngine/graphics/tkBlur.h"

namespace tkEngine{
	class CPostEffect;
	/*!
	*@brief	Screen Space Reflection
	*@details
	* スクリーン空間でレイマーチングを行い疑似的な映り込みを表現します。
	*/
	class CScreenSpaceReflection{
	public:
		/*!
		*@brief	開放。
		*/
		void Release();
		/*!
		*@brief	初期化。
		*/
		void Init(const SGraphicsConfig& config);
		/*!
		*@brief	描画。
		*@param[in]		rc		レンダリングコンテキスt。
		*/
		void Render(CRenderContext& rc, CPostEffect* postEffect);
	private:
		/*!
		*@brief 定数バッファ。
		*@todo 明日は定数バッファから
		*/
		struct SConstantBuffer {
			CMatrix mViewProjInv;			//!<ビュープロジェクション行列の逆行列。
			CMatrix mViewProj;				//!<ビュープロジェクション行列。
			CMatrix mViewProjInvLastFrame;	//!<1フレーム前のビュープロジェクション行列の逆行列。
			CVector4 cameraPos;				//!<カメラの視点。
			CVector4 renderTargteSize;		//!<レンダリングターゲットのサイズ。
			float rayMarchStepRate;			//!<レイマーチのステップレート。
		};
		bool m_isEnable = false;					//!<有効。
		CShader m_vsShader;							//!<頂点シェーダー。
		CShader m_psShader;							//!<ピクセルシェーダー。
		CShader m_psFinalShader;					//!<最終合成シェーダー。
		CConstantBuffer m_cb;						//!<定数バッファ。
		std::random_device m_rd;					//!<他に影響を与えないように、こいつが内部で乱数を保持する。
		std::mt19937 m_mt;							//!<メルセンヌツイスターを使用した乱数生成器。
		int m_currentRTNo = 0;						//!<
		static const int NUM_CALC_AVG_RT = 1;				//!<平均輝度計算用のレンダリングターゲットの枚数。
		CRenderTarget m_reflectionRT[NUM_CALC_AVG_RT];		//!<リフレクションを書き込むレンダリングターゲット。
		CBlur m_blur;
		CMatrix m_viewProjInvLastFrame = CMatrix::Identity;
	};
}