/*!
 * @brief	ブラー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkShadowBlur.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine{
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	CShadowBlur::CShadowBlur()
	{
	}
	CShadowBlur::~CShadowBlur()
	{
	}
	void CShadowBlur::Init( CShaderResourceView& srcTexture, float blurIntensity, const SShadowRenderConfig& shadowConfig)
	{
		m_srcTexture = &srcTexture;
		m_blurIntensity = blurIntensity;
		//SRVに関連付けされているテクスチャの情報を取得。
		D3D11_TEXTURE2D_DESC desc;
		srcTexture.GetTextureDesc(desc);
		
		m_srcTextureWidth = desc.Width;
		m_srcTextureHeight = desc.Height;

		DXGI_SAMPLE_DESC multiSampleDesc;
		ZeroMemory(&multiSampleDesc, sizeof(multiSampleDesc));
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		//Xブラー用のレンダリングターゲットを作成。
		m_xBlurRT.Create(
			desc.Width / 2,
			desc.Height,
			desc.MipLevels,
			desc.ArraySize,
			desc.Format,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		//Yブラー用のレンダリングターゲットを作成。
		m_yBlurRT.Create(
			desc.Width / 2,
			desc.Height / 2,
			desc.MipLevels,
			desc.ArraySize,
			desc.Format,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		m_cbBlur.Create(&m_blurParam, sizeof(m_blurParam));

		//頂点バッファのソースデータ。
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };

		m_fullscreenQuad.Create(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			4,
			sizeof(SSimpleVertex),
			vertices,
			4,
			CIndexBuffer::enIndexType_16,
			indices
		);
		//シェーダーをロード。
		m_vsBlurShader.Load("shader/shadowBlur.fx", "VSBlur", CShader::EnType::VS);
		m_psXBlurShader.Load("shader/shadowBlur.fx", "PSXBlur", CShader::EnType::PS);
		m_psYBlurShader.Load("shader/shadowBlur.fx", "PSYBlur", CShader::EnType::PS);

		m_blurParam.offsetTexelWorld = shadowConfig.offsetTexelWorld;

	}
	void CShadowBlur::UpdateWeight(float dispersion)
	{
		float total = 0;
		for (int i = 0; i<NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
			total += 2.0f*m_blurParam.weights[i];

		}
		// 規格化
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] /= total;
		}
	}
	void CShadowBlur::Execute(CRenderContext& rc)
	{
		//ウェイトの更新
		UpdateWeight(m_blurIntensity);
		m_blurParam.mtxProj = MainCamera().GetProjectionMatrix();
		m_blurParam.mtxProjInv.Inverse(m_blurParam.mtxProj);
		
		CRenderTarget* oldRenderTargets[MRT_MAX];
		unsigned int numRenderTargetViews;

		rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);
		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		ID3D11DepthStencilState* depthStenciil = rc.GetDepthStencilState();
		rc.OMSetDepthStencilState(DepthStencilState::spriteRender);
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.VSSetShader(m_vsBlurShader);
		rc.PSSetShaderResource(1, GetGBufferSRV(enGBufferDepth));
		//XBlur
		{
			CRenderTarget* rts[] = {
				&m_xBlurRT
			};
			rc.UpdateSubresource(m_cbBlur, &m_blurParam);
			rc.OMSetRenderTargets(1, rts);
			rc.ClearRenderTargetView(0, clearColor);
			rc.VSSetShaderResource(0, *m_srcTexture);
			rc.PSSetShaderResource(0, *m_srcTexture);
			rc.PSSetConstantBuffer(0, m_cbBlur);
			rc.RSSetViewport(0.0f, 0.0f, (float)m_xBlurRT.GetWidth(), (float)m_xBlurRT.GetHeight());
			
			rc.PSSetShader(m_psXBlurShader);

			m_fullscreenQuad.Draw(rc);
		}
		//YBlur
		{
			CRenderTarget* rts[] = {
				&m_yBlurRT
			};
			rc.PSUnsetShaderResource(enSkinModelSRVReg_SoftShadowMap);
			rc.UpdateSubresource(m_cbBlur, &m_blurParam);
			rc.OMSetRenderTargets(1, rts);
			rc.ClearRenderTargetView(0, clearColor);
			rc.VSSetShaderResource(0, m_xBlurRT.GetRenderTargetSRV());
			rc.PSSetShaderResource(0, m_xBlurRT.GetRenderTargetSRV());
			rc.PSSetConstantBuffer(0, m_cbBlur);
			rc.RSSetViewport(0.0f, 0.0f, (float)m_yBlurRT.GetWidth(), (float)m_yBlurRT.GetHeight());
			rc.PSSetShader(m_psYBlurShader);

			m_fullscreenQuad.Draw(rc);
		}
		rc.OMSetDepthStencilState(depthStenciil);
		//レンダリングターゲットを戻す。
		rc.OMSetRenderTargets(numRenderTargetViews, oldRenderTargets);
		rc.RSSetViewport(0.0f, 0.0f, (float)oldRenderTargets[0]->GetWidth(), (float)oldRenderTargets[0]->GetHeight());
	}
}