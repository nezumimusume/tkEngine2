/*!
 * @brief	プリレンダー
 */

#ifndef _TKPRERENDER_H_
#define _TKPRERENDER_H_

#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/graphics/preRender/tkLightCulling.h"
#include "tkEngine/graphics/preRender/shadow/tkShadowMap.h"
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
		 *@brief	指向性シャドウマップを取得。
		 */
		CDirectionShadowMap& GetDirectionShadowMap()
		{
			return m_directionShadowMap;
		}
		/*!
		 *@brief	G-Bufferを取得。
		 */
		CGBufferRender& GetGBufferRender()
		{
			return m_gbufferRender;
		}
	private:
		SGraphicsConfig		m_config;				//!<コンフィグ。
		CLightCulling		m_lightCulling;			//!<ライトカリング。
		CDirectionShadowMap	m_directionShadowMap;	//!<指向性シャドウマップ。
		CGBufferRender		m_gbufferRender;		//!<G-Buffer。
	};
}
#endif //_TKPRERENDER_H_