/*!
 *@brief	六角形ブラー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkHexaBlur.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

namespace tkEngine {
	void CHexaBlur::Init(CShaderResourceView& srcTexture, bool isScaleup)
	{
		D3D11_TEXTURE2D_DESC texDesc;
		m_srcTexture = &srcTexture;
		m_srcTexture->GetTextureDesc(texDesc);

		for (auto& rt : m_renderTarget) {
			DXGI_SAMPLE_DESC multiSampleDesc;
			multiSampleDesc.Count = 1;
			multiSampleDesc.Quality = 0;

			rt.Create(
				isScaleup == true ? texDesc.Width * 2 : texDesc.Width,
				isScaleup == true ? texDesc.Height * 2 : texDesc.Height,
				1,
				1,
				texDesc.Format,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			);
		}
		//シェーダーを初期化。
		m_vs.Load("shader/hexaBlur.fx", "VSMain", CShader::EnType::VS);
		m_psVerticalDiagonalBlur.Load("shader/hexaBlur.fx", "PSVerticalDiagonalBlur", CShader::EnType::PS);
		m_psCombineVerticalDiagonalBlur.Load("shader/hexaBlur.fx", "PSCombineVerticalDiagonalBlur", CShader::EnType::PS);
		m_psRhomboidBlur.Load("shader/hexaBlur.fx", "PSRhomboidBlur", CShader::EnType::PS);
		//定数バッファを初期化。
		m_cb.Create(nullptr, sizeof(SCBParam));
	}
	void CHexaBlur::Execute(CRenderContext& rc, std::function<void(CRenderContext& rc, EnRenderPass)> onPreDraw)
	{
		if (m_srcTexture == nullptr) {
			//ソーステクスチャが指定されていないぞゴルァ。
			return;
		}
		CGraphicsEngine& ge = Engine().GetGraphicsEngine();
		ge.BeginGPUEvent(L"CHexaBlur::Execute");

		D3D11_TEXTURE2D_DESC texDesc;
		m_srcTexture->GetTextureDesc(texDesc);
		SCBParam cbParam;
		cbParam.texSize.x = texDesc.Width;
		cbParam.texSize.y = texDesc.Height;
		cbParam.radius = m_radius;	
		//VRAM上の定数バッファを更新。
		rc.UpdateSubresource(m_cb, &cbParam);
		//b0レジスタに定数バッファのアドレスを設定。
		rc.PSSetConstantBuffer(0, m_cb);

		//ポストエフェクトの全画面描画の機能を使いたいので、ポストエフェクトの
		//インスタンスを取得する。
		auto& postEffect = GraphicsEngine().GetPostEffect();

		//レンダリングステートとレンダリングターゲットを退避させる。
		rc.PushRenderState();
		rc.PushRenderTargets();

		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.OMSetBlendState(AlphaBlendState::disable);
		//垂直、斜めブラー
		{
			CRenderTarget* rts[] = {
				&m_renderTarget[enRenderTarget_VerticalBlur],
				&m_renderTarget[enRenderTarget_DiagonalBlur]
			};
			CChangeRenderTarget chgRt(rc, 2, rts);
			rc.PSSetShaderResource(0, *m_srcTexture);
			rc.VSSetShader(m_vs);
			rc.PSSetShader(m_psVerticalDiagonalBlur);
			float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			rc.ClearRenderTargetView(0, color);
			rc.ClearRenderTargetView(1, color);
			if (onPreDraw != nullptr) {
				onPreDraw(rc, enRenderPass_VerticalDiagonalBlur);
			}
			postEffect.DrawFullScreenQuad(rc);
		}
		//垂直ブラーと斜めブラーを合成する。
	/*	{
			CChangeRenderTarget chgRt(rc, m_renderTarget[enRenderTarget_VerticalDiagonalBlur]);
			rc.PSSetShaderResource(0, m_renderTarget[enRenderTarget_VerticalBlur].GetRenderTargetSRV());
			rc.PSSetShaderResource(1, m_renderTarget[enRenderTarget_DiagonalBlur].GetRenderTargetSRV());
			rc.VSSetShader(m_vs);
			rc.PSSetShader(m_psCombineVerticalDiagonalBlur);
			if (onPreDraw != nullptr) {
				onPreDraw(rc, enRenderPass_CombineVerticalDiagonalBlur);
			}
			postEffect.DrawFullScreenQuad(rc);
		}*/
		//六角形ブラーを作成する。
		{
			CChangeRenderTarget chgRt(rc, m_renderTarget[enRenderTarget_RhomboidBlur]);
			rc.PSSetShaderResource(0, m_renderTarget[enRenderTarget_VerticalBlur].GetRenderTargetSRV());
			rc.PSSetShaderResource(1, m_renderTarget[enRenderTarget_DiagonalBlur].GetRenderTargetSRV());
			rc.VSSetShader(m_vs);
			rc.PSSetShader(m_psRhomboidBlur);
			if (onPreDraw != nullptr) {
				onPreDraw(rc, enRenderPass_RhomboidBlur);
			}
			postEffect.DrawFullScreenQuad(rc);
		}
		//レンダリングステートとレンダリングターゲットを戻す。
		rc.PopRenderState(true);
		rc.PopRenderTargets(true);

		ge.EndGPUEvent();
	}
}