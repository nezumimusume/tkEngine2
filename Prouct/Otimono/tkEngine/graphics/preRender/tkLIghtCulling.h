/*!
 *@brief	ライトカリング
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	ライトカリング。
	 *@details
	 * TBFRの動的光源のタイルベースのカリングの処理。
	 */
	class CLightCulling{
	public:
		static const int TILE_WIDTH = 16;
		/*!
		 *@brief	コンストラクタ。
		 */
		CLightCulling();
		/*!
		 *@brief	デストラクタ。
		 */
		~CLightCulling();
		/*!
		*@brief	初期化。
		*/
		void Init();
		/*!
		 *@brief	ライトカリングの実行。
		 */
		void Render(CRenderContext& rc);
	private:
		//カメラパラメータ。
		struct SCameraParam {
			CMatrix mProj;			//プロジェクション行列。
			CMatrix mProjInv;		//プロジェクション行列の逆行列。
			CMatrix mViewRot;		//カメラの回転行列の逆行列。
			CVector4 screenParam;	//スクリーンパラメータ。
			int numPointLight;		//ポイントライトの数。
		};
		
		SCameraParam	m_cameraParam;			//カメラパラメータ。
		CConstantBuffer m_cameraParamCB;	//カメラ用の定数バッファ。
		CShader			m_csLightCulling;		//!<ライトカリング用のコンピュートシェーダー。
	};
}