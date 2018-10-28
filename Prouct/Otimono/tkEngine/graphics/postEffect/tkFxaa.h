/*!
 *@brief	FXAA
 */

#pragma once

namespace tkEngine{
	class CPostEffect;
	/*!
	 *@brief	FXAA
	 *@details
	 * ポストエフェクトによるアンチエイリアス。
	 */
	class CFxaa : Noncopyable{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CFxaa();
		/*!
		 *@brief	デストラクタ。
		 */
		~CFxaa();
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
		*@param[in]		rc		レンダリングコンテキスト。
		*/
		void Render(CRenderContext& rc, CPostEffect* postEffect);
	private:
		bool m_isEnable = false;	//!<有効？
		CShader m_vsShader;			//!<頂点シェーダー
		CShader m_psShader;			//!<FXAA用のピクセルシェーダー。
		CSamplerState m_samplerState;	//!<FXAA用のサンプラステート。
	};
}