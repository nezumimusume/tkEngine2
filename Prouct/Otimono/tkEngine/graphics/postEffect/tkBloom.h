/*!
 * @brief	ブルーム
 */
#pragma once

namespace tkEngine{
	class CPostEffect;
	/*!
	 * @brief	ブルーム
	 */
	class CBloom : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CBloom();
		/*!
		 * @brief	デストラクタ。
		 */
		~CBloom();
		/*!
		 * @brief	開放
		 */
		void Release();
		/*!
		 * @brief	初期化。
		 */
		void Init( const SGraphicsConfig& config );
		/*!
		 * @brief	描画
		 */
		void Render(CRenderContext& rc, CPostEffect* postEffect);
	private:
		/*!
		* @brief	ガウスフィルタの重みを更新。
		*/
		void UpdateWeight(float dispersion);
	private:
		
		static const int NUM_WEIGHTS = 8;
		static const int NUM_DOWN_SAMPLING_RT = 10;
		/*!
		* @brief	ブラー用のパラメータバッファ。
		*@details
		* これを変更したら、Assets/shader/bloom.fxのCBBlurの中身も変更するように。
		*/
		struct SBlurParam {
			CVector4 offset;
			float weights[NUM_WEIGHTS];
		};
		bool m_isEnable = false;		//!<有効。
		CRenderTarget m_luminanceRT;	//!<輝度を抽出するレンダリングターゲット。
		CRenderTarget m_combineRT;		//!<ぼかし合成用のレンダリングターゲット。
		CRenderTarget m_downSamplingRT[NUM_DOWN_SAMPLING_RT];	//!<ダウンサンプリング用のレンダリングターゲット。
		CShader m_vsShader;	
		CShader m_psLuminance;			//!<輝度抽出用のピクセルシェーダー。
		CShader m_vsXBlur;				//!<XBlur用の頂点シェーダー。
		CShader m_vsYBlur;				//!<YBlur用の頂点シェーダー。
		CShader m_psBlur;				//!<XBlur用のピクセルシェーダー。
		CShader m_psCombine;			//!<合成用のピクセルシェーダー。
		CShader	m_copyVS;				//!<コピー用の頂点シェーダー。
		CShader	m_copyPS;				//!<コピー用のピクセルシェーダー。
		SBlurParam m_blurParam;			//!<ブラー用のパラメータ。
		CConstantBuffer m_cbBlur;
		CConstantBuffer m_cb;
	};
}