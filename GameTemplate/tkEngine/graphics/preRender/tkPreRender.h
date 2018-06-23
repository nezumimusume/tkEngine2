/*!
 * @brief	プリレンダー
 */

#ifndef _TKPRERENDER_H_
#define _TKPRERENDER_H_

#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/graphics/preRender/tkLightCulling.h"
#include "tkEngine/graphics/preRender/tkShadowMap.h"
#include "tkEngine/graphics/preRender/tkGBufferRender.h"

namespace tkEngine{
	/*!
	 * @brief	プリレンダリング。
	 */
	class CPreRender : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPreRender();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPreRender();
		/*!
		* @brief	開放
		*/
		void Release();
		/*!
		 * @brief	作成。
		 *@param[in]	config	コンフィグ。
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& rc);
		/*!
		* @brief	更新。
		*/
		void Update();
		
		/*!
		 *@brief	シャドウマップを取得。
		 */
		CShadowMap& GetShadowMap()
		{
			return m_shadowMap;
		}
		/*!
		 *@brief	G-Bufferを取得。
		 */
		CGBufferRender& GetGBufferRender()
		{
			return m_gbufferRender;
		}
	private:
		SGraphicsConfig	m_config;			//!<コンフィグ。
		CLightCulling	m_lightCulling;		//!<ライトカリング。
		CShadowMap		m_shadowMap;		//!<シャドウマップ。
		CGBufferRender	m_gbufferRender;	//!<G-Buffer。
	};
}
#endif //_TKPRERENDER_H_