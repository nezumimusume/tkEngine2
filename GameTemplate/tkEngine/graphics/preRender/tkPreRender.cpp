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
		m_directionShadowMap.Init(config.shadowRenderConfig);
		m_gbufferRender.Init(config);
		m_ominiDirectionShadowMap.Init(config.ominiShadowRenderConfig);
	}
	
	void CPreRender::Update() 
	{
		m_directionShadowMap.Update();
	}
	
	void CPreRender::Render(CRenderContext& rc)
	{
		m_ominiDirectionShadowMap.RenderToShadowMap(rc);
		m_directionShadowMap.RenderToShadowMap(rc);
		m_gbufferRender.Render(rc);
		m_lightCulling.Render(rc);
	}

}

