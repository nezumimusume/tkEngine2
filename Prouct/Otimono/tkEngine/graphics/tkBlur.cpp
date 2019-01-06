/*!
 * @brief	ブラー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkBlur.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

namespace tkEngine{
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	CBlur::CBlur()
	{
	}
	CBlur::~CBlur()
	{
	}
	void CBlur::InitScaleup(CShaderResourceView& srcTexture, float blurIntensity )
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
			desc.Width * 2,
			desc.Height,
			desc.MipLevels,
			desc.ArraySize,
			desc.Format,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		//Yブラー用のレンダリングターゲットを作成。
		m_yBlurRT.Create(
			desc.Width * 2,
			desc.Height * 2,
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
		m_vsXBlurShader.Load("shader/blur.fx", "VSXBlur", CShader::EnType::VS);
		m_vsYBlurShader.Load("shader/blur.fx", "VSYBlur", CShader::EnType::VS);
		m_psBlurShader.Load("shader/blur.fx", "PSBlur", CShader::EnType::PS);
	}
	void CBlur::Init( CShaderResourceView& srcTexture, float blurIntensity )
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
		m_vsXBlurShader.Load("shader/blur.fx", "VSXBlur", CShader::EnType::VS);
		m_vsYBlurShader.Load("shader/blur.fx", "VSYBlur", CShader::EnType::VS);
		m_psBlurShader.Load("shader/blur.fx", "PSBlur", CShader::EnType::PS);

	}
	void CBlur::UpdateWeight(float dispersion)
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
	void CBlur::Execute(
		CRenderContext& rc, 
		std::function<void(CRenderContext&, EnRenderStep)> onPreDraw
	){
		//ウェイトの更新
		UpdateWeight(m_blurIntensity);

		//現在のレンダリングターゲットを退避させる。
		rc.PushRenderTargets();
		//現在のレンダリングステートを退避させる。
		rc.PushRenderState();

		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		ID3D11DepthStencilState* depthStenciil = rc.GetDepthStencilState();
		rc.OMSetDepthStencilState(DepthStencilState::spriteRender);
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		//XBlur
		{
			CChangeRenderTarget chgRt(rc, m_xBlurRT);
			rc.ClearRenderTargetView(0, clearColor);
			m_blurParam.offset.x = 16.0f / m_srcTextureWidth;
			m_blurParam.offset.y = 0.0f;
			rc.UpdateSubresource(m_cbBlur, &m_blurParam);
			rc.VSSetShaderResource(0, *m_srcTexture);
			rc.PSSetShaderResource(0, *m_srcTexture);
			rc.PSSetConstantBuffer(0, m_cbBlur);
			rc.VSSetConstantBuffer(0, m_cbBlur);
			rc.VSSetShader(m_vsXBlurShader);
			rc.PSSetShader(m_psBlurShader);

			if (onPreDraw != nullptr) {
				onPreDraw(rc, enRenderStep_XBlur);
			}
			m_fullscreenQuad.Draw(rc);
		}
		////YBlur
		{
			CChangeRenderTarget chgRt(rc, m_yBlurRT);
			rc.ClearRenderTargetView(0, clearColor);
			
			m_blurParam.offset.x = 0.0f;
			m_blurParam.offset.y = 16.0f / m_srcTextureHeight;
		
			rc.UpdateSubresource(m_cbBlur, &m_blurParam);
			rc.VSSetShaderResource(0, m_xBlurRT.GetRenderTargetSRV());
			rc.PSSetShaderResource(0, m_xBlurRT.GetRenderTargetSRV());
			rc.PSSetConstantBuffer(0, m_cbBlur);
			rc.VSSetConstantBuffer(0, m_cbBlur);

			rc.VSSetShader(m_vsYBlurShader);
			rc.PSSetShader(m_psBlurShader);

			if (onPreDraw != nullptr) {
				onPreDraw(rc, enRenderStep_YBlur);
			}
			m_fullscreenQuad.Draw(rc);
		}
		//レンダリングステートを戻す。
		rc.PopRenderState(true);
		//レンダリングターゲットを戻す。
		rc.PopRenderTargets(true);
	}
}