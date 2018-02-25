/*!
 * @brief	ブラー。
 */

#pragma once

#include "tkEngine/graphics/tkPrimitive.h"

namespace tkEngine{
	/*!
	 * @brief	ブラー。
	 */
	class CBlur : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CBlur();
		/*!
		 * @brief	デストラクタ。
		 */
		~CBlur();
		/*!
		 * @brief	初期化。
		 *@param[in]	srcTexture		元テクスチャ。
		 *@param[in]	blurIntensity	ブラーの強さ。値が大きいほどボケる。
		 */
		void Init( CShaderResourceView& srcTexture, float blurIntensity = 25.0f );
		/*!
		 * @brief	ブラーを実行。
		 */
		void Execute(CRenderContext& rc);
		/*!
		* @brief	実行結果を格納しているSRVを取得。
		*/
		CShaderResourceView& GetResultSRV()
		{
			return m_yBlurRT.GetRenderTargetSRV();
		}
	private:
		/*!
		* @brief	ガウスフィルタの重みを更新。
		*/
		void UpdateWeight(float dispersion);
	private:
		static const int NUM_WEIGHTS = 8;
		/*!
		* @brief	ブラー用のパラメータバッファ。
		*@details
		* これを変更したら、Assets/shader/blur.fxのCBBlurの中身も変更するように。
		*/
		struct SBlurParam {
			CVector4 offset;
			float weights[NUM_WEIGHTS];
		};
		SBlurParam m_blurParam;				//!<ブラー用のパラメータ。
		CConstantBuffer m_cbBlur;			//!<ブラー用のパラメータの定数バッファ。
		CShaderResourceView* m_srcTexture;	//!<ソーステクスチャ。
		CRenderTarget m_xBlurRT;			//!<Xブラーの描き込み先。
		CRenderTarget m_yBlurRT;			//!<Yブラーの書き込み先。
		CShader m_vsXBlurShader;			//!<Xブラー用の頂点シェーダー。
		CShader m_vsYBlurShader;			//!<Yブラー用の頂点シェーダー。
		CShader m_psBlurShader;				//!<ブラー用のピクセルシェーダー。
		int m_srcTextureWidth = 0;			//!<ソーステクスチャの幅。
		int m_srcTextureHeight = 0;			//!<ソーステクスチャの高さ。
		CPrimitive	m_fullscreenQuad;		//!<フルスクリーン描画用の矩形プリミティブ。
		CSamplerState m_samplerState;		//!<サンプラステート。@todo ひとまとめにした方がいい?。
		float m_blurIntensity = 25.0f;
	};
}