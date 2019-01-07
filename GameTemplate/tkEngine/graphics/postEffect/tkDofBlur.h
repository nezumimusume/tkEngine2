/*!
 * @brief	被写界深度用のブラー。
 */

#pragma once

#include "tkEngine/graphics/tkBlur.h"
#include "tkEngine/graphics/tkHexaBlur.h"

namespace tkEngine{
	/*!
	 * @brief	被写界深度用のブラー。
	 */
	class CDofBlur : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CDofBlur();
		/*!
		 * @brief	デストラクタ。
		 */
		~CDofBlur();
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
		/// <summary>
		/// 六角形ブラーの結果を取得。
		/// </summary>
		/// <returns></returns>
		CShaderResourceView& GetHexaBlurResultSRV()
		{
			return m_hexaBlur.GetResultSRV();
		}
		/// <summary>
		/// 六角形ボケの半径を設定する。
		/// </summary>
		/// <param name="r">半径。テクセル空間。デフォルトは8です。</param>
		void SetHexaBokeRadius(float r)
		{
			m_hexaBlur.SetRadius(r);
		}
		/// <summary>
		/// 六角形ボケの半径を取得。
		/// </summary>
		float GetHexaBokeRadius() const
		{
			return m_hexaBlur.GetRadius();
		}
	private:
		bool m_isScaleupBlur = false;
		CShaderResourceView* m_srcTexture = nullptr;
		CRenderTarget m_downSamplingRT;
		CConstantBuffer m_cb;
		CShader m_vsMiniBlur;				//小規模ブラーの頂点シェーダー。
		CShader m_psMiniBlur;				//小規模ブラーのピクセルシェーダー。
		CShader m_vsXBlurShader;			//!<Xブラー用の頂点シェーダー。
		CShader m_vsYBlurShader;			//!<Yブラー用の頂点シェーダー。
		CShader m_psBlurShader;				//!<ブラー用のピクセルシェーダー。
		CShader m_psVerticalDiagonalBlur;	//垂直斜めブラー。
		CBlur m_blur;						//!<ガウシアンブラー。
		CHexaBlur m_hexaBlur;				//!<六角形ブラー。
	};
}