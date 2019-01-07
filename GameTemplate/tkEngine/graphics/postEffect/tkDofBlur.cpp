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
	void CDofBlur::Init( CShaderResourceView& srcTexture, float blurIntensity )
	{
		m_srcTexture = &srcTexture;
		//まずは1/4の解像度への小規模な縮小ブラー。
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		D3D11_TEXTURE2D_DESC texDesc;
		srcTexture.GetTextureDesc(texDesc);
		m_downSamplingRT.Create(
			texDesc.Width / 4,
			texDesc.Height / 4,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		
		//続いて小規模ブラーのテクスチャを利用した拡大六角形ブラー。
		m_hexaBlur.Init(m_downSamplingRT.GetRenderTargetSRV(), true);
		m_hexaBlur.SetRadius(4.0f);
		m_vsMiniBlur.Load("shader/dof/dof_CreateBokeTexture.fx", "VSMinBlur", CShader::EnType::VS);
		m_psMiniBlur.Load("shader/dof/dof_CreateBokeTexture.fx", "PSMinBlur", CShader::EnType::PS);
		m_psVerticalDiagonalBlur.Load("shader/dof/dof_CreateBokeTexture.fx", "PSVerticalDiagonalBlur", CShader::EnType::PS);
		m_cb.Create(nullptr, sizeof(CVector4));
		
	}
	void CDofBlur::Execute(CRenderContext& rc)
	{
		//1/2の解像度への小規模な縮小ブラー。
		CVector2 invRenderTargetSize;
		invRenderTargetSize.x = 1.0f / m_downSamplingRT.GetWidth();
		invRenderTargetSize.y = 1.0f / m_downSamplingRT.GetHeight();
		rc.UpdateSubresource(m_cb, &invRenderTargetSize);
		rc.VSSetConstantBuffer(0, m_cb);
		rc.VSSetShader(m_vsMiniBlur);
		rc.PSSetShader(m_psMiniBlur);
		rc.PSSetShaderResource(0, *m_srcTexture);
		CChangeRenderTarget chgRt(rc, m_downSamplingRT);
			
		GraphicsEngine().GetPostEffect().DrawFullScreenQuad(rc);

		//1/4の解像度への六角形の縮小ブラーをかける。
		m_hexaBlur.Execute(rc, [&](CRenderContext& rc, CHexaBlur::EnRenderPass renderPass) {
			if (renderPass == CHexaBlur::enRenderPass_VerticalDiagonalBlur) {
				//ピクセルシェーダー差し替え。
				rc.PSSetShader(m_psVerticalDiagonalBlur);
			}
		});
	}
}