/*!
 *@brief	Screen Space Reflection
 */

#pragma once

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
			CMatrix mViewProjInv;		//!<ビュープロジェクション行列の逆行列。
			CMatrix mViewProj;			//!<ビュープロジェクション行列。
			CVector4 cameraPos;			//!<カメラの視点。
		};
		bool m_isEnable = false;		//!<有効。
		CShader m_vsShader;				//!<頂点シェーダー。
		CShader m_psShader;				//!<ピクセルシェーダー。
		CConstantBuffer m_cb;			//!<定数バッファ。
	};
}