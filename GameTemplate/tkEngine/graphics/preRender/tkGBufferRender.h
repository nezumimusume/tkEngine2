/*!
 * @brief	G-Bufferを描画。
 */

#pragma once

#include "tkEngine/graphics/tkShadowBlur.h"

namespace tkEngine{
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
		 * @brief	描画。
		 */
		void Render(CRenderContext& rc);
		/*!
		*@brief	GBufferのパラメータをGPUに転送。
		*/
		void SendGBufferParamToGPU(CRenderContext& rc);
	private:
		enum EnGBuffer{
			enGBufferNormal,	//!<法線。
			enGBufferShadow,	//!<影マップ。
			enGBufferNum,		//!<G-Bufferの数。
		};
		//定数パラメータ。
		struct SCBParam {
			bool isPCFShadowMap;		//影マップを作るときにPCFを行う。
		};
		std::vector<CSkinModel*>	m_skinModels;	//!<スキンモデルのリスト。
		CRenderTarget m_GBuffer[enGBufferNum];		//!<G-Buffer
		CShadowBlur m_shadowBlur;					//!<影マップへのブラー処理。
		SCBParam m_cbEntity;
		CConstantBuffer m_cb;
	};
}
