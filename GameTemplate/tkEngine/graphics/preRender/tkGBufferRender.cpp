/*!
 * @brief	G-Bufferの描画。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkGBufferRender.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CGBufferRender::CGBufferRender()
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CGBufferRender::~CGBufferRender()
	{
	}
	/*!
	*@brief	初期化。
	*/
	void CGBufferRender::Init(const SGraphicsConfig& config)
	{
		CGraphicsEngine& ge = GraphicsEngine();
		
		//法線バッファの初期化。
		m_GBuffer[enGBufferNormal].Create(
			ge.GetFrameBufferWidth(),
			ge.GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_SNORM,
			DXGI_FORMAT_UNKNOWN,
			ge.GetMainRenderTargetMSAADesc()
		);
		

		//影マップの初期化。
		m_GBuffer[enGBufferShadow].Create(
			ge.GetFrameBufferWidth(),
			ge.GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			ge.GetMainRenderTargetMSAADesc()
		);

		//Zバッファはメインレンダリングターゲットのものを使用する。
		m_GBuffer[0].SetDepthStencilView(
			ge.GetMainRenderTarget().GetDepthStencilView()
		);

		m_shadowBlur.Init(m_GBuffer[enGBufferShadow].GetRenderTargetSRV(), 5.0f, config.shadowRenderConfig);
		
		m_cb.Create(NULL, sizeof(m_cbEntity));
	}
	
	void CGBufferRender::SendGBufferParamToGPU(CRenderContext& rc)
	{
		if (GraphicsEngine().GetShadowMap().GetSoftShadowLevel() == EnSoftShadowQualityLevel::enNone) {
			//ハードシャドウ。
			rc.PSSetShaderResource(enSkinModelSRVReg_SoftShadowMap, m_GBuffer[enGBufferShadow].GetRenderTargetSRV());
		}
		else {
			//ソフトシャドウ
			rc.PSSetShaderResource(enSkinModelSRVReg_SoftShadowMap, m_shadowBlur.GetResultSRV());
		}
	}
	
	void CGBufferRender::Render(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_RenderGBuffer");
		EnSoftShadowQualityLevel ssLevel = GraphicsEngine().GetShadowMap().GetSoftShadowLevel();

		//影を落とすための情報を転送。
		GraphicsEngine().GetShadowMap().SendShadowReceiveParamToGPU(rc);

		rc.SetRenderStep(enRenderStep_RenderGBuffer);

		m_cbEntity.isPCFShadowMap = ssLevel == EnSoftShadowQualityLevel::eSSSS_PCF;
		rc.UpdateSubresource(m_cb, &m_cbEntity);

		rc.PSSetConstantBuffer(enSkinModelCBReg_GBuffer, m_cb);
		//レンダリングターゲットのバックアップを取得する。
		CRenderTarget* oldRenderTargets[MRT_MAX];
		unsigned int numRenderTargetViews;
		rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);

		//レンダリングターゲットを変更する。
		CRenderTarget* renderTargets[] = {
			&m_GBuffer[enGBufferNormal],
			&m_GBuffer[enGBufferShadow],
		};
		rc.OMSetRenderTargets(enGBufferNum, renderTargets);
		//Shadowバッファをクリア。
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		rc.ClearRenderTargetView(1, clearColor);

		for (auto& skinModel : m_skinModels) {
			skinModel->Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
		}
		//MSAAリゾルブ。
		for (auto& rt : renderTargets) {
			rt->ResovleMSAATexture(rc);
		}

		if (ssLevel == EnSoftShadowQualityLevel::eSSSS
			|| ssLevel == EnSoftShadowQualityLevel::eSSSS_PCF
		) {
			//スクリーンスペースソフトシャドウ。
			//影マップにブラーをかける。
			m_shadowBlur.Execute(rc);
		}

		rc.OMSetRenderTargets(numRenderTargetViews, oldRenderTargets);
		m_skinModels.clear();

		EndGPUEvent();
	}
}