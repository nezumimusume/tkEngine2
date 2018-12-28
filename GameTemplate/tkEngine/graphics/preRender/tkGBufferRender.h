/*!
 * @brief	G-Bufferを描画。
 */

#pragma once

#include "tkEngine/graphics/tkShadowBlur.h"

namespace tkEngine{
	//!<G-Bufferの定義。
	enum EnGBuffer{
		enGBufferAlbedo,		//!<アルベド。
		enGBufferNormal,		//!<法線。
		enGBufferSpecular,		//!<スペキュラ。
		enGBufferShadow,		//!<影マップ。
		enGBufferDepth,			//!<深度。xには射影空間での深度値、yにはカメラ空間での深度値が記録されている。
		enGBufferTangent,		//!<接法線。
		enGBufferMateriaID,		//!<マテリアルID。
		enGBufferEmission,		//!<自己発光色。
		enGBufferNum,			//!<G-Bufferの数。
	};
	/*!
	 * @brief	G-Buffer。
	 */
	class CGBufferRender : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CGBufferRender();
		/*!
		 * @brief	デストラクタ。
		 */
		~CGBufferRender();
		/*!
		 *@brief	初期化。
		 */
		void Init(const SGraphicsConfig& config);
		/*!
		*@brief	スキンモデルを追加。
		*/
		void AddSkinModel(CSkinModel* skinModel)
		{
			m_skinModels.push_back(skinModel);
		}
		/*!
		*@brief	スキンモデルを除去。
		*/
		void RemoveSkinModel(CSkinModel* skinModel)
		{
			auto it = std::find(m_skinModels.begin(), m_skinModels.end(), skinModel);
			if (it != m_skinModels.end() ){
				m_skinModels.erase(it);
			}
		}
		/*!
		 * @brief	描画。
		 */
		void Render(CRenderContext& rc);
		/*!
		*@brief	GBufferのパラメータをレジスタに設定。
		*/
		void SetGBufferParamToReg(CRenderContext& rc);
		/*!
		*@brief	GBufferをレジスタから外す。
		*/
		void UnsetGBufferParamFromReg(CRenderContext& rc);
		/*!
		*@brief	GBufferを取得。
		*@param[in]	enGBuffer	取得したいG-Buffer
		*/
		CRenderTarget& GetRenderTarget(EnGBuffer enGBuffer) 
		{
			return m_GBuffer[enGBuffer];
		}
		CShaderResourceView& GetDepthTextureLastFrameSRV()
		{
			return m_depthTextureLastFrameSRV;
		}
	private:
		
		//定数パラメータ。
		struct SCBParam {
			bool isPCFShadowMap;		//影マップを作るときにPCFを行う。
		};
		std::vector<CSkinModel*>	m_skinModels;	//!<スキンモデルのリスト。
		CRenderTarget m_GBuffer[enGBufferNum];		//!<G-Buffer
		ID3D11Texture2D* m_depthTextureLastFrame;		//!<1フレーム前の深度テクスチャ。
		CShaderResourceView m_depthTextureLastFrameSRV;	//!<1フレーム前の深度テクスチャのSRV。
		CShadowBlur m_shadowBlur;					//!<影マップへのブラー処理。
		SCBParam m_cbEntity;
		CConstantBuffer m_cb;
	};
}
