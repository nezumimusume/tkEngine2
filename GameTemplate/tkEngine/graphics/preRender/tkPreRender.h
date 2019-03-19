#ifndef _TKPRERENDER_H_
#define _TKPRERENDER_H_

#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/graphics/preRender/tkLightCulling.h"
#include "tkEngine/graphics/preRender/shadow/tkDirectionShadowMap.h"
#include "tkEngine/graphics/preRender/tkGBufferRender.h"
#include "tkEngine/graphics/preRender/shadow/tkOminiDirectionShadowMap.h"

namespace tkEngine{
	
	/// <summary>
	/// プリレンダリング。
	/// </summary>
	class CPreRender : Noncopyable{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CPreRender();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CPreRender();
		
		/// <summary>
		/// 関数。
		/// </summary>
		void Release();
		
		/// <summary>
		/// 作成。
		/// </summary>
		/// <param name="config"></param>
		void Create( const SGraphicsConfig& config );
		
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc"></param>
		void Render(CRenderContext& rc);
		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		
		/// <summary>
		/// 指向性シャドウマップを取得。
		/// </summary>
		/// <returns></returns>
		CDirectionShadowMap& GetDirectionShadowMap()
		{
			return m_directionShadowMap;
		}
		/// <summary>
		/// 全方位シャドウマップを取得。
		/// </summary>
		/// <returns></returns>
		COminiDirectionShadowMap& GetOminiDirectionShadowMap()
		{
			return m_ominiDirectionShadowMap;
		}
		
		/// <summary>
		/// G-Bufferを取得。
		/// </summary>
		/// <returns></returns>
		CGBufferRender& GetGBufferRender()
		{
			return m_gbufferRender;
		}
	private:
		SGraphicsConfig		m_config;				//コンフィグ。
		CLightCulling		m_lightCulling;			//ライトカリング。
		CDirectionShadowMap	m_directionShadowMap;	//指向性シャドウマップ。
		COminiDirectionShadowMap m_ominiDirectionShadowMap;	//全方位シャドウマップ。
		CGBufferRender		m_gbufferRender;		//G-Buffer。
	};
}
#endif //_TKPRERENDER_H_