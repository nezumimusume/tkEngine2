/*!
 * @brief	ディザリング。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	ディザリング。
	 * @details
	 *  最終的な画像にノイズを混ぜることによって、マッハバンドなどの
	 *  アーティファクトを軽減する。
	 */
	class CDithering : Noncopyable{
	public:
		CDithering();
		~CDithering();
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
		bool m_isEnable = false;		//!<有効？
		CShader m_vsShader;				//!<頂点シェーダー
		CShader m_psShader;				//!<ピクセルシェーダー。
		CSamplerState m_samplerState;	//!<サンプラステート。
	};
}