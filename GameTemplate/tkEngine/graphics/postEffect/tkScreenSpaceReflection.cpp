/*!
 *@brief	Screen space reflection
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkScreenSpaceReflection.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkPresetSamplerState.h"

namespace tkEngine {
	void CScreenSpaceReflection::Release()
	{
	}
	void CScreenSpaceReflection::Init(const SGraphicsConfig& config)
	{
		m_isEnable = config.ssrConfig.isEnable;
		m_vsShader.Load("shader/ScreenSpaceReflection.fx", "VSMain", CShader::EnType::VS);
		m_psShader.Load("shader/ScreenSpaceReflection.fx", "PSMain", CShader::EnType::PS);
	}
	/*!
	*@brief	描画。
	*@param[in]		rc		レンダリングコンテキスt。
	*/
	void CScreenSpaceReflection::Render(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (m_isEnable == false) {
			return;
		}
		BeginGPUEvent(L"enRenderStep_ScreenSpaceReflection");
		rc.OMSetDepthStencilState(DepthStencilState::disable, 0);
		//現在のレンダリングターゲットを取得。
		CRenderTarget& rt = postEffect->GetFinalRenderTarget();
		//レンダリングターゲットを切り替える。
		postEffect->ToggleFinalRenderTarget();
		CRenderTarget* renderTargets[] = {
			&postEffect->GetFinalRenderTarget()
		};

		rc.OMSetBlendState(AlphaBlendState::disable, 0, 0xFFFFFFFF);
		rc.PSSetSampler(0, *CPresetSamplerState::sampler_clamp_clamp_clamp_linear);
		rc.OMSetRenderTargets(1, renderTargets);
		rc.PSSetShaderResource(0, rt.GetRenderTargetSRV());
		rc.PSSetShader(m_psShader);
		rc.VSSetShader(m_vsShader);
		//入力レイアウトを設定。
		rc.IASetInputLayout(m_vsShader.GetInputLayout());

		postEffect->DrawFullScreenQuad(rc);
		rc.OMSetDepthStencilState(DepthStencilState::SceneRender, 0);
		rc.PSUnsetShaderResource(0);
		EndGPUEvent();
	}
}

