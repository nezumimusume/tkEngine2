/*!
 * @brief	ブルーム
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/graphics/postEffect/tkBloom.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CBloom::CBloom()
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CBloom::~CBloom()
	{
		Release();
	}
	/*!
	 * @brief	開放
	 */
	void CBloom::Release()
	{
	}
	/*!
	 * @brief	初期化。
	 */
	void CBloom::Init( const SGraphicsConfig& config )
	{
		Release();
		
		if(!config.bloomConfig.isEnable){
			return ;
		}
		m_vsShader.Load("shader/bloom.fx", "VSMain", CShader::EnType::VS);
		m_psLuminance.Load("shader/bloom.fx", "PSSamplingLuminance", CShader::EnType::PS);
		m_vsXBlur.Load("shader/bloom.fx", "VSXBlur", CShader::EnType::VS);
		m_vsYBlur.Load("shader/bloom.fx", "VSYBlur", CShader::EnType::VS);
		m_psBlur.Load("shader/bloom.fx", "PSBlur", CShader::EnType::PS);
		m_psCombine.Load("shader/bloom.fx", "PSCombine", CShader::EnType::PS);
		m_copyVS.Load("shader/copy.fx", "VSMain", CShader::EnType::VS);
		m_copyPS.Load("shader/copy.fx", "PSMain", CShader::EnType::PS);
		int w = Engine().GetGraphicsEngine().GetFrameBufferWidth();
		int h = Engine().GetGraphicsEngine().GetFrameBufferHeight();

		//輝度抽出用のレンダリングターゲットを作成。
		DXGI_SAMPLE_DESC multiSampleDesc;
		ZeroMemory(&multiSampleDesc, sizeof(multiSampleDesc));
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		if (!m_luminanceRT.Create(
			w,
			h,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		)) {
			TK_WARNING("Failed CBloom::Init");
			return;
		}

		//ダウンサンプリング用のレンダリングターゲットを作成する。
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
			int shift = i + 1;
			int baseIndex = i * 2;
			//横ブラー用。
			if (!m_downSamplingRT[baseIndex].Create(
				w >> shift,
				h >> (shift - 1),
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			)) {
				TK_WARNING("Failed CBloom::Init");
				return ;
			}
			//縦ブラー用。
			if (!m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			)) {
				TK_WARNING("Failed CBloom::Init");
				return;
			}
		}
		//ぼかし合成用。
		if (!m_combineRT.Create(
			w >> 2,
			h >> 2,
			1, 1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		)) {
			TK_WARNING("Failed CBloom::Init");
			return;
		}
		m_cbBlur.Create(&m_blurParam, sizeof(m_blurParam));
		m_isEnable = config.bloomConfig.isEnable;

		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_samplerState.Create(desc);

	}
	void CBloom::UpdateWeight(float dispersion)
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
	void CBloom::Render(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable) {
			return;
		}
		UpdateWeight(25.0f);
		rc.SetRenderStep(enRenderStep_Bloom);
		CGraphicsEngine& ge = Engine().GetGraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Bloom");
		rc.PSSetSampler(0, m_samplerState);
		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		//輝度抽出。
		{
			ge.BeginGPUEvent(L"enRenderStep_Bloom::Luminance");
			CRenderTarget* rts[] = {
				&m_luminanceRT
			};
			
			CRenderTarget& finalRT = postEffect->GetFinalRenderTarget();
			

			rc.OMSetRenderTargets(1, rts);
			rc.ClearRenderTargetView(0, clearColor);
			rc.PSSetShaderResource(0, finalRT.GetRenderTargetSRV());
			rc.VSSetShader(m_vsShader);
			//入力レイアウトを設定。
			rc.IASetInputLayout(m_vsShader.GetInputLayout());
			rc.PSSetShader(m_psLuminance);
			postEffect->DrawFullScreenQuad(rc);
			ge.EndGPUEvent();
		}
		//輝度をぼかす。
		{
			ge.BeginGPUEvent(L"enRenderStep_Bloom::Blur");
			CRenderTarget* prevRt = &m_luminanceRT;

			int rtIndex = 0;
			for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
				//XBlur
				{
					CRenderTarget* rts[] = {
						&m_downSamplingRT[rtIndex]
					};
					m_blurParam.offset.x = 16.0f / prevRt->GetWidth();
					m_blurParam.offset.y = 0.0f;
					rc.UpdateSubresource(m_cbBlur, &m_blurParam);
					rc.OMSetRenderTargets(1, rts);
					rc.ClearRenderTargetView(0, clearColor);
					rc.VSSetShaderResource(0, prevRt->GetRenderTargetSRV());
					rc.PSSetShaderResource(0, prevRt->GetRenderTargetSRV());
					rc.PSSetConstantBuffer(0, m_cbBlur);
					rc.RSSetViewport(0.0f, 0.0f, (float)m_downSamplingRT[rtIndex].GetWidth(), (float)m_downSamplingRT[rtIndex].GetHeight());
					rc.VSSetShader(m_vsXBlur);
					rc.PSSetShader(m_psBlur);
					postEffect->DrawFullScreenQuad(rc);
				}

				prevRt = &m_downSamplingRT[rtIndex];
				rtIndex++;
				//YBlur
				{
					CRenderTarget* rts[] = {
						&m_downSamplingRT[rtIndex]
					};
					m_blurParam.offset.x = 0.0f;
					m_blurParam.offset.y = 16.0f / prevRt->GetWidth();
					rc.UpdateSubresource(m_cbBlur, &m_blurParam);
					rc.PSSetConstantBuffer(0, m_cbBlur);
					rc.OMSetRenderTargets(1, rts);
					rc.ClearRenderTargetView(0, clearColor);
					rc.VSSetShaderResource(0, prevRt->GetRenderTargetSRV());
					rc.PSSetShaderResource(0, prevRt->GetRenderTargetSRV());
					rc.RSSetViewport(0.0f, 0.0f, (float)m_downSamplingRT[rtIndex].GetWidth(), (float)m_downSamplingRT[rtIndex].GetHeight());
					rc.VSSetShader(m_vsYBlur);
					rc.PSSetShader(m_psBlur);
					postEffect->DrawFullScreenQuad(rc);
				}
				prevRt = &m_downSamplingRT[rtIndex];
				rtIndex++;
			}
			ge.EndGPUEvent();
		}
		//ボケ画像の作成。
		{
			
			CRenderTarget* rts[] = {
				&m_combineRT
			};
			//
			rc.OMSetBlendState(AlphaBlendState::disable, 0, 0xFFFFFFFF);

			rc.RSSetViewport(0.0f, 0.0f, (float)m_combineRT.GetWidth(), (float)m_combineRT.GetHeight());
			rc.OMSetRenderTargets(1, rts);
			rc.ClearRenderTargetView(0, clearColor);

			rc.PSSetShaderResource(0, m_downSamplingRT[3].GetRenderTargetSRV());
			rc.PSSetShaderResource(1, m_downSamplingRT[5].GetRenderTargetSRV());
			rc.PSSetShaderResource(2, m_downSamplingRT[7].GetRenderTargetSRV());
			rc.PSSetShaderResource(3, m_downSamplingRT[9].GetRenderTargetSRV());
			rc.VSSetShader(m_vsShader);
			rc.PSSetShader(m_psCombine);
			postEffect->DrawFullScreenQuad(rc);
		}
		//最終合成。
		{
			CRenderTarget& finalRT = postEffect->GetFinalRenderTarget();
			CRenderTarget* rts[] = {
				&finalRT
			};
			rc.RSSetViewport(0.0f, 0.0f, finalRT.GetWidth(), finalRT.GetHeight());
			rc.OMSetRenderTargets(1, rts);
			// アルファブレンディングを加算合成にする。
			rc.OMSetBlendState(AlphaBlendState::add, 0, 0xFFFFFFFF);
			
			//定数バッファを設定する。
			rc.VSSetShader(m_copyVS);
			rc.PSSetShader(m_copyPS);
			rc.PSSetShaderResource(0, m_combineRT.GetRenderTargetSRV());
			postEffect->DrawFullScreenQuad(rc);

			//アルファブレンディングをもとに戻す。
			rc.OMSetBlendState(AlphaBlendState::trans, 0, 0xFFFFFFFF);
		}

		rc.PSUnsetShaderResource(0);
		rc.PSUnsetShaderResource(1);
		rc.PSUnsetShaderResource(2);
		rc.PSUnsetShaderResource(3);

		Engine().GetGraphicsEngine().EndGPUEvent();
	}
}