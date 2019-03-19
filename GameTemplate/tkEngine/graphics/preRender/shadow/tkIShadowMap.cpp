/// <summary>
/// シャドウマップの基底クラス。
/// </summary>

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/shadow/tkIShadowMap.h"
#include "tkEngine/graphics/preRender/shadow/tkShadowCaster.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"


namespace tkEngine {
	void IShadowMap::RenderToShadowMap(CRenderContext& rc)
	{
		//シャドウマップへの書き込み処理を呼び出す。
		RenderToShadowMapImp(rc);
		//シャドウキャスターをクリア。
		m_shadowCaster.clear();
	}
}
