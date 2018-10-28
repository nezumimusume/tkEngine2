/*!
 * @brief	プリレンダリング。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkPreRender.h"

namespace tkEngine{
	CPreRender::CPreRender()
	{
	}
	CPreRender::~CPreRender()
	{
		Release();
	}
	void CPreRender::Release()
	{
	}
	void CPreRender::Create( const SGraphicsConfig& config )
	{
		m_lightCulling.Init();
		m_shadowMap.Init(config.shadowRenderConfig);
		m_gbufferRender.Init(config);
	}
	
	void CPreRender::Update() 
	{
		m_shadowMap.Update();
	}
	
	void CPreRender::Render(CRenderContext& rc)
	{
		m_shadowMap.RenderToShadowMap(rc);
		m_gbufferRender.Render(rc);
		m_lightCulling.Render(rc);
	}

}

