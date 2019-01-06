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
		m_psFinalShader.Load("shader/ScreenSpaceReflection.fx", "PSFinal", CShader::EnType::PS);
		m_cb.Create(nullptr, sizeof(SConstantBuffer));
		m_mt.seed(m_rd());
		//輝度抽出用のレンダリングターゲットを作成。
		DXGI_SAMPLE_DESC multiSampleDesc;
		ZeroMemory(&multiSampleDesc, sizeof(multiSampleDesc));
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;

		CRenderTarget& mrt = GraphicsEngine().GetMainRenderTarget();
		for (auto& rt : m_reflectionRT) {
			rt.Create(
				mrt.GetWidth() >> 1,
				mrt.GetHeight() >> 1 ,
				1,
				1,
				mrt.GetRenderTargetTextureFormat(),
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			);
		}
		m_blur.Init(m_reflectionRT[0].GetRenderTargetSRV(), 5.0f);
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
		//レンダリングステートを退避させる。
		rc.PushRenderState();

		rc.OMSetDepthStencilState(DepthStencilState::disable);
		
		CRenderTarget& rt = postEffect->GetFinalRenderTarget();

		CChangeRenderTarget chgRt(rc, m_reflectionRT[m_currentRTNo]);
		m_currentRTNo = (m_currentRTNo + 1) % NUM_CALC_AVG_RT;
		
		
		//定数バッファを更新して、レジスタに設定。
		SConstantBuffer cb;
		cb.cameraPos = MainCamera().GetPosition();
		cb.mViewProj = MainCamera().GetViewProjectionMatrix();
		cb.mViewProjInv.Inverse(MainCamera().GetViewProjectionMatrix());
		cb.mViewProjInvLastFrame = m_viewProjInvLastFrame;
		cb.renderTargteSize.x = GraphicsEngine().GetFrameBufferWidth();
		cb.renderTargteSize.y = GraphicsEngine().GetFrameBufferHeight();
		cb.rayMarchStepRate = (float)(m_mt()%1000000)/1000000.0f;
		rc.UpdateSubresource(m_cb, &cb);
		rc.PSSetConstantBuffer(0, m_cb);
		m_viewProjInvLastFrame = cb.mViewProjInv;
		CGBufferRender& gBuffer = GraphicsEngine().GetGBufferRender();

		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.OMSetBlendState(AlphaBlendState::trans);
	
		rc.PSSetShaderResource(0, rt.GetRenderTargetSRV());
		rc.PSSetShaderResource(1, gBuffer.GetRenderTarget(enGBufferNormal).GetRenderTargetSRV());
		rc.PSSetShaderResource(2, gBuffer.GetRenderTarget(enGBufferDepth).GetRenderTargetSRV());
		rc.PSSetShaderResource(3, gBuffer.GetDepthTextureLastFrameSRV());
		rc.PSSetShader(m_psShader);
		rc.VSSetShader(m_vsShader);
		postEffect->DrawFullScreenQuad(rc);
		
		rc.OMSetBlendState(AlphaBlendState::disable);

		m_blur.Execute(rc);

		//戻す。
		////レンダリングターゲットを切り替える。
		postEffect->ToggleFinalRenderTarget();
		{
			CChangeRenderTarget chgRt(rc, postEffect->GetFinalRenderTarget());
		
			rc.PSSetConstantBuffer(0, m_cb);
			rc.PSSetShaderResource(0, rt.GetRenderTargetSRV());
			rc.PSSetShaderResource(1, m_blur.GetResultSRV());
			/*for (int i = 0; i < NUM_CALC_AVG_RT; i++) {
				rc.PSSetShaderResource(1 + i, m_reflectionRT[i].GetRenderTargetSRV());

			}*/
			rc.PSSetShaderResource(2, gBuffer.GetRenderTarget(enGBufferSpecular).GetRenderTargetSRV());

			//最終合成
			rc.VSSetShader(m_vsShader);
			rc.PSSetShader(m_psFinalShader);
			postEffect->DrawFullScreenQuad(rc);
			for (int i = 0; i < NUM_CALC_AVG_RT; i++) {
				rc.PSUnsetShaderResource(1 + i);
			}
		}
	
		rc.PSUnsetShaderResource(0);
		//レンダリングステートを元に戻す。
		rc.PopRenderState(true);

		EndGPUEvent();
	}
}

