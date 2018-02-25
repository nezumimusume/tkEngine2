/*!
 *@brief	トーンマップ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkTonemap.h"
#include "tkEngine/graphics/tkPresetRenderState.h"


namespace tkEngine{
	namespace {
		
		//-----------------------------------------------------------------------------
		// Name: GetSampleOffsets_DownScale4x4
		// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
		//       pixel shader.
		//-----------------------------------------------------------------------------
		HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, CVector4 avSampleOffsets[])
		{
			if (NULL == avSampleOffsets)
				return E_INVALIDARG;

			float tU = 1.0f / dwWidth;
			float tV = 1.0f / dwHeight;

			// Sample from the 16 surrounding points. Since the center point will be in
			// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
			// center.
			int index = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					avSampleOffsets[index].x = (x - 1.5f) * tU;
					avSampleOffsets[index].y = (y - 1.5f) * tV;

					index++;
				}
			}

			return S_OK;
		}
	}
	CTonemap::CTonemap()
	{
	}
	CTonemap::~CTonemap()
	{
	}
	void CTonemap::Init(const SGraphicsConfig& config)
	{
		if (!config.tonemapConfig.isEnable) {
			return;
		}
		m_isEnable = true;
		DXGI_SAMPLE_DESC multiSampleDesc;
		ZeroMemory(&multiSampleDesc, sizeof(multiSampleDesc));
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
	//	m_effect = EffectManager().LoadEffect("Assets/presetShader/tonemap.fx");
		//平均輝度計算用のレンダリングターゲットを作成。
		for (int i = 0; i < NUM_CALC_AVG_RT; i++)
		{
			int rtSize = 1 << (2 * i);
			m_calcAvgRT[i].Create(
				rtSize,
				rtSize,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			);
		}

		//最終的にトーンマップで使用する平均輝度を書き込むレンダリングターゲットを作成。
		for (auto& rt : m_avgRT) {
			rt.Create(
				1,
				1,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			);
		}
		m_tonemapParam.midddleGray = config.tonemapConfig.luminance;
		m_cbTonemapCommon.Create(&m_tonemapParam, sizeof(m_tonemapParam));
		m_vsShader.Load("shader/tonemap.fx", "VSMain", CShader::EnType::VS);
		m_psCalcLuminanceLogAvarageShader.Load("shader/tonemap.fx", "PSCalcLuminanceLogAvarage", CShader::EnType::PS);
		m_psCalcLuminanceAvarageShader.Load("shader/tonemap.fx", "PSCalcLuminanceAvarage", CShader::EnType::PS);
		m_psCalcLuminanceExpAvarageShader.Load("shader/tonemap.fx", "PSCalcLuminanceExpAvarage", CShader::EnType::PS);
		m_psCalcAdaptedLuminanceShader.Load("shader/tonemap.fx", "PSCalcAdaptedLuminance", CShader::EnType::PS);
		m_psCalcAdaptedLuminanceFirstShader.Load("shader/tonemap.fx", "PSCalcAdaptedLuminanceFirst", CShader::EnType::PS);
		m_psFinal.Load("shader/tonemap.fx", "PSFinal", CShader::EnType::PS);
		m_cbCalcLuminanceLog.Create(m_avSampleOffsets, sizeof(m_avSampleOffsets));

		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_samplerState.Create(desc);
	}
	void CTonemap::CalcLuminanceAvarage(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable) {
			return;
		}
		CGraphicsEngine& ge = Engine().GetGraphicsEngine();
		ge.BeginGPUEvent(L"CTonemap::CalcLuminanceAvarage");
		
		
		//対数平均を求める。
		float tU, tV;
		tU = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetWidth());
		tV = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetHeight());

		int index = 0;
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				m_avSampleOffsets[index].x = x * tU;
				m_avSampleOffsets[index].y = y * tV;

				index++;
			}
		}
		int curRtNo = NUM_CALC_AVG_RT - 1;
		{
			ge.BeginGPUEvent(L"CTonemap::CalcLuminanceLogAvarage");
			
			CRenderTarget* rts[] = {
				&m_calcAvgRT[curRtNo]
			};
			rc.OMSetRenderTargets(1, rts);
			rc.RSSetViewport(0.0f, 0.0f, (float)m_calcAvgRT[curRtNo].GetWidth(), (float)m_calcAvgRT[curRtNo].GetHeight());
			rc.IASetInputLayout(m_vsShader.GetInputLayout());
			rc.UpdateSubresource(m_cbCalcLuminanceLog, m_avSampleOffsets);
			rc.PSSetConstantBuffer(0, m_cbCalcLuminanceLog);
			rc.VSSetShader(m_vsShader);
			rc.PSSetShader(m_psCalcLuminanceLogAvarageShader);

			rc.PSSetShaderResource(0, GraphicsEngine().GetMainRenderTarget().GetRenderTargetSRV());
			postEffect->DrawFullScreenQuad(rc);
			ge.EndGPUEvent();
		}
		//ダウンサンプリングを行って平均を求める。
		curRtNo--;
		{
			ge.BeginGPUEvent(L"CTonemap::CalcLuminanceAvarage");
			while (curRtNo > 0) {
				CRenderTarget* rts[] = {
					&m_calcAvgRT[curRtNo]
				};
				rc.OMSetRenderTargets(1, rts);
				rc.RSSetViewport(0.0f, 0.0f, (float)m_calcAvgRT[curRtNo].GetWidth(), (float)m_calcAvgRT[curRtNo].GetHeight());
				GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
				rc.UpdateSubresource(m_cbCalcLuminanceLog, m_avSampleOffsets);
				rc.PSSetConstantBuffer(0, m_cbCalcLuminanceLog);
				rc.PSSetShader(m_psCalcLuminanceAvarageShader);
				rc.PSSetShaderResource(0, m_calcAvgRT[curRtNo + 1].GetRenderTargetSRV());
				postEffect->DrawFullScreenQuad(rc);
				curRtNo--;
			}
			ge.EndGPUEvent();
		}
		//exp関数を用いて最終平均を求める。
		CRenderTarget* rts[] = {
			&m_calcAvgRT[curRtNo]
		};
		rc.OMSetRenderTargets(1, rts);
		GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
		{
			ge.BeginGPUEvent(L"CTonemap::CalcLuminanceExpAvarage");
			rc.RSSetViewport(0.0f, 0.0f, (float)m_calcAvgRT[curRtNo].GetWidth(), (float)m_calcAvgRT[curRtNo].GetHeight());
			rc.UpdateSubresource(m_cbCalcLuminanceLog, m_avSampleOffsets);
			rc.PSSetConstantBuffer(0, m_cbCalcLuminanceLog);
			rc.PSSetShaderResource(0, m_calcAvgRT[curRtNo + 1].GetRenderTargetSRV());	
			rc.PSSetShader(m_psCalcLuminanceExpAvarageShader);
			postEffect->DrawFullScreenQuad(rc);
			ge.EndGPUEvent();
		}
		//明暗順応。
		{
			ge.BeginGPUEvent(L"CTonemap::CalcAdaptedLuminance");
			if (m_isFirstWhenChangeScene == true) {
				//シーンが切り替わって初回。
				m_currentAvgRT = 1 ^ m_currentAvgRT;
				CRenderTarget* rts[] = {
					&m_avgRT[m_currentAvgRT]
				};
				rc.RSSetViewport(0.0f, 0.0f, (float)m_avgRT[m_currentAvgRT].GetWidth(), (float)m_avgRT[m_currentAvgRT].GetHeight());
				rc.OMSetRenderTargets(1, rts);
				rc.PSSetShaderResource(1, m_calcAvgRT[0].GetRenderTargetSRV());
				rc.PSSetShader(m_psCalcAdaptedLuminanceFirstShader);
				postEffect->DrawFullScreenQuad(rc);
				m_isFirstWhenChangeScene = false;
			}
			else {
					
				CRenderTarget& lastRT = m_avgRT[m_currentAvgRT];
				m_currentAvgRT = 1 ^ m_currentAvgRT;
				CRenderTarget* rts[] = {
					&m_avgRT[m_currentAvgRT]
				};
				rc.OMSetRenderTargets(1, rts);
				rc.RSSetViewport(0.0f, 0.0f, (float)m_avgRT[m_currentAvgRT].GetWidth(), (float)m_avgRT[m_currentAvgRT].GetHeight());
				float deltaTime = GameTime().GetFrameDeltaTime();
				rc.PSSetShaderResource(1, m_calcAvgRT[0].GetRenderTargetSRV());
				rc.PSSetShaderResource(2, lastRT.GetRenderTargetSRV());
				rc.PSSetShader(m_psCalcAdaptedLuminanceShader);
				postEffect->DrawFullScreenQuad(rc);
				
			}
			ge.EndGPUEvent();
		}
		ge.EndGPUEvent();
		
	}
	void CTonemap::Render(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable) {
			return;
		}
		rc.SetRenderStep(enRenderStep_Toonmap);
		CGraphicsEngine& ge = Engine().GetGraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Toonmap");
		m_tonemapParam.deltaTime = GameTime().GetFrameDeltaTime();
		rc.UpdateSubresource(m_cbTonemapCommon, &m_tonemapParam);
		rc.PSSetConstantBuffer(1, m_cbTonemapCommon);
		rc.PSSetSampler(0, m_samplerState);
		CalcLuminanceAvarage(rc, postEffect);

		
		CShaderResourceView& sceneSRV = GraphicsEngine().GetMainRenderTarget().GetRenderTargetSRV();
		
		CRenderTarget& rt = GraphicsEngine().GetMainRenderTarget();
		CRenderTarget* rts[] = {
			&rt
		};
		rc.RSSetViewport(0.0f, 0.0f, rt.GetWidth(), rt.GetHeight());
		rc.OMSetRenderTargets(1, rts);
		rc.PSSetShaderResource(0, sceneSRV);
		rc.PSSetShaderResource(1, m_avgRT[m_currentAvgRT].GetRenderTargetSRV());
		rc.OMSetBlendState(AlphaBlendState::disable, 0, 0xFFFFFFFF);
		rc.OMSetDepthStencilState(DepthStencilState::disable, 0);
		
		rc.PSSetShader(m_psFinal);
		postEffect->DrawFullScreenQuad(rc);
		
		//戻す。
		rc.OMSetDepthStencilState(DepthStencilState::SceneRender, 0);

		rc.PSUnsetShaderResource(0);
		rc.PSUnsetShaderResource(1);
		ge.EndGPUEvent();
	}
}