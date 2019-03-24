/*!
 * @brief	全方位シャドウマップ
 */

#pragma once

#include "tkEngine/graphics/preRender/shadow/tkIShadowMap.h"
#include <array>

namespace tkEngine{
	class IShadowCaster;
	/*!
	 * @brief	全方位シャドウマップ。
	 */
	class COminiDirectionShadowMap : public IShadowMap{
	public:
		
		/*!
		 * @brief	コンストラクタ。
		 */
		COminiDirectionShadowMap();
		/*!
		 * @brief	デストラクタ。
		 */
		~COminiDirectionShadowMap();
		/*!
		 * @brief	シャドウマップの作成。
		 *@param[in]	config		コンフィグ。
		 *@return	trueが返ってきたらシャドウマップの作成成功。
		 */
		bool Init( const SOminiShadowRenderConfig& config );
		/*!
		* @brief	更新。
		*/
		void Update();
		/*!
		 *@brief	影を落とすためのパラメータをGPUに転送する。
		 */
		void SendShadowReceiveParamToGPU(CRenderContext& rc);
		
		/// <summary>
		/// ライトの座標を設定する。
		/// </summary>
		/// <param name="pos">ライトの座標</param>
		void SetLightPosition(CVector3 pos)
		{
			m_ominiDirectionShadowCbCPU.lightPosition = pos;
		}
		/// <summary>
		/// ライトの影響を受ける距離を設定する。
		/// </summary>
		/// <param name="distance">距離</param>
		void SetDistanceAffectedByLight(float distance)
		{
			m_ominiDirectionShadowCbCPU.distanceAffectedByLight = distance;
		}
		/// <summary>
		/// 近平面までのクリップ距離を設定。
		/// </summary>
		/// <param name="n">近平面までの距離</param>
		void SetNearClip(float n)
		{
			m_near = n;
		}
		/// <summary>
		/// シャドウバイアスを設定。
		/// </summary>
		/// <remarks>
		/// セルフシャドウで発生するシャドウアクネを回避するための
		/// パラメータです。大きな値を入れると影が落ちにくくなります。
		/// </remarks>
		/// <param name="bias"></param>
		void SetShadowBias(float bias)
		{
			m_ominiDirectionShadowCbCPU.shadowBias = bias;
		}
	private:
		/// <summary>
		/// シャドウマップ用のテクスチャを作成。
		/// </summary>
		bool InitShadowMapTextures(ID3D11Device* pD3DDevice);
		/// <summary>
		/// レンダリングターゲットビューを作成。
		/// </summary>
		/// <returns></returns>
		bool InitRenderTargetViews(ID3D11Device* pD3DDevice);
		/// <summary>
		/// デプスステンシルテクスチャを作成。
		/// </summary>
		/// <returns></returns>
		bool InitDepthStencilTextures(ID3D11Device* pD3DDevice);
		/// <summary>
		/// デプスステンシルビューを初期化。
		/// </summary>
		/// <param name="pD3DDevice"></param>
		/// <returns></returns>
		bool InitDepthStencilViews(ID3D11Device* pD3DDevice);
		/*!
		 *@brief	シャドウマップへ書き込み。
		 */
		void RenderToShadowMapImp(CRenderContext& rc) override;
	private:
		enum { NUM_TEXTURE = 6 }; //テクスチャの数。
		/// <summary>
		/// 定数バッファ構造体
		/// </summary>
		/// <remarks>
		/// GPUに送りたいパラメータはこの構造体に入れる。
		/// この構造体のメンバを変更したら、shader/modelCB.hの
		/// OminiDirectionShadowCbも変更するように。
		/// </remarks>
		struct SOminiDirectionShadowCb {
			CMatrix lightViewProjMatrix[NUM_TEXTURE] = {CMatrix::Identity};	//ライトビュープロジェクション行列。
			CVector3 lightPosition = CVector3::Zero;	//光源の座標。
			float distanceAffectedByLight = 100;		//ライトの影響を受ける距離。
			float shadowBias = 0.001f;					//シャドウバイアス。
		};
		
		ID3D11Texture2D* m_shadowMap = nullptr;	//シャドウマップ。動的キューブマップ。6面レンダリングを行うことで、全方位シャドウを実現する。
		std::array< ID3D11RenderTargetView*, NUM_TEXTURE> m_renderTargets;	//レンダリングターゲット。
		ID3D11Texture2D* m_depthStencilTexture = nullptr;					//デプスステンシルテクスチャ。
		ID3D11DepthStencilView* m_depthStencilView = nullptr;				//デプスステンシルビュー。	
		D3D11_TEXTURE2D_DESC m_texDesc = { 0 };								//テクスチャ定義。
		D3D11_TEXTURE2D_DESC m_depthTexDesc = { 0 };						//デプスステンシルテクスチャの定義。
		CShaderResourceView m_shadowMapSRV;									//シャドウマップのSRV。
		SOminiDirectionShadowCb m_ominiDirectionShadowCbCPU;				//CPU側の定数バッファ
		CConstantBuffer m_ominiDirectionShadowCbGPU;						//GPU側の定数バッファ。
		float m_near = 1.0f;
	};
}