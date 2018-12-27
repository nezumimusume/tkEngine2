/*!
 * @brief	被写界深度用のブラー。
 */

#pragma once

#include "tkEngine/graphics/tkBlur.h"

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
		/*!
		* @brief	実行結果を格納しているSRVを取得。
		*/
		CShaderResourceView& GetResultSRV()
		{
			return m_blur.GetResultSRV();
		}
	
	private:
	
		CShader m_vsXBlurShader;			//!<Xブラー用の頂点シェーダー。
		CShader m_vsYBlurShader;			//!<Yブラー用の頂点シェーダー。
		CShader m_psBlurShader;				//!<ブラー用のピクセルシェーダー。
		CBlur m_blur;						//!<ガウシアンブラー。
	};
}