/*!
 * @brief	被写界深度用のブラー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkDofBlur.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

namespace tkEngine{

	CDofBlur::CDofBlur()
	{
	}
	CDofBlur::~CDofBlur()
	{
	}
	void CDofBlur::Init( CShaderResourceView& srcTexture, float blurIntensity, bool isScaleupBlur )
	{
		m_isScaleupBlur = isScaleupBlur;
		m_srcTexture = &srcTexture;
		if (isScaleupBlur) {
			//このフラグがtrueの場合は一旦ダウンサンプリングを行って
			//そこから拡大ブラーを行う。
			//縮小バッファのアーティファクトの軽減のため。
			DXGI_SAMPLE_DESC multiSampleDesc;
			multiSampleDesc.Count = 1;
			multiSampleDesc.Quality = 0;
			D3D11_TEXTURE2D_DESC texDesc;
			srcTexture.GetTextureDesc(texDesc);
			m_downSamplingRT.Create(
				texDesc.Width / 2,
				texDesc.Height / 2,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN,
				multiSampleDesc
			);
			//ダウンサンプリングしたテクスチャに対して拡大ブラー。
			m_blur.InitScaleup(m_downSamplingRT.GetRenderTargetSRV(), blurIntensity);

			m_vsDownSample.Load("shader/dof/dof_CreateBokeTexture.fx", "VSDownSample", CShader::EnType::VS);
			m_psDownSample.Load("shader/dof/dof_CreateBokeTexture.fx", "PSDownSample", CShader::EnType::PS);
			m_cb.Create(nullptr, sizeof(CVector4));
		}
		else {
			//こちらは縮小ブラー。
			m_blur.Init(srcTexture, blurIntensity);
		}
		//シェーダーをロード。
		m_vsXBlurShader.Load("shader/dof/dof_CreateBokeTexture.fx", "VSXBlur", CShader::EnType::VS);
		m_vsYBlurShader.Load("shader/dof/dof_CreateBokeTexture.fx", "VSYBlur", CShader::EnType::VS);
		m_psBlurShader.Load("shader/dof/dof_CreateBokeTexture.fx", "PSBlur", CShader::EnType::PS);
	}
	void CDofBlur::Execute(CRenderContext& rc)
	{
		if (m_isScaleupBlur == true) {
			//一旦ダウンサンプリング。
			CVector2 invRenderTargetSize;
			invRenderTargetSize.x = 1.0f / m_downSamplingRT.GetWidth();
			invRenderTargetSize.y = 1.0f / m_downSamplingRT.GetHeight();
			rc.UpdateSubresource(m_cb, &invRenderTargetSize);
			rc.VSSetConstantBuffer(0, m_cb);
			rc.VSSetShader(m_vsDownSample);
			rc.PSSetShader(m_psDownSample);
			rc.PSSetShaderResource(0, *m_srcTexture);
			CChangeRenderTarget chgRt(rc, m_downSamplingRT);
			
			GraphicsEngine().GetPostEffect().DrawFullScreenQuad(rc);
		}
		m_blur.Execute(rc, [&](CRenderContext& rc, CBlur::EnRenderStep enRenderStep) {
			if (enRenderStep == CBlur::enRenderStep_XBlur) {
				//Xブラー。
				rc.VSSetShader(m_vsXBlurShader);
				rc.PSSetShader(m_psBlurShader);
			}
			else if (enRenderStep == CBlur::enRenderStep_YBlur) {
				//Yブラー。
				rc.VSSetShader(m_vsYBlurShader);
				rc.PSSetShader(m_psBlurShader);
			}
		});
	}
}