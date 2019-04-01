/*!
 * @brief	シャドウマップ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/shadow/tkOminiDirectionShadowMap.h"
#include "tkEngine/graphics/preRender/shadow/tkShadowCaster.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine {

	COminiDirectionShadowMap::COminiDirectionShadowMap()
	{
	}

	COminiDirectionShadowMap::~COminiDirectionShadowMap()
	{
		if (m_shadowMap != nullptr) {
			m_shadowMap->Release();
		}
		for (auto& rt : m_renderTargets) {
			rt->Release();
		}
		if (m_depthStencilTexture != nullptr) {
			m_depthStencilTexture->Release();
		}
		if (m_depthStencilView != nullptr) {
			m_depthStencilView->Release();
		}
	}
	bool COminiDirectionShadowMap::InitShadowMapTextures(ID3D11Device* pD3DDevice)
	{
		m_texDesc.Width = 512;
		m_texDesc.Height = 512;
		m_texDesc.MipLevels = 1;
		m_texDesc.ArraySize = NUM_TEXTURE;
		m_texDesc.Format = DXGI_FORMAT_R32_FLOAT;
		m_texDesc.SampleDesc.Count = 1;
		m_texDesc.SampleDesc.Quality = 0;
		m_texDesc.Usage = D3D11_USAGE_DEFAULT;
		m_texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_texDesc.CPUAccessFlags = 0;
		m_texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		auto hr = pD3DDevice->CreateTexture2D(&m_texDesc, nullptr, &m_shadowMap);
		if (FAILED(hr)) {
			//キューブマップの作成に失敗。
			return false;
		}
		//SRVも作成。
		if (m_shadowMapSRV.Create(m_shadowMap) == false) {
			return false;
		}
		return true;
	}
	bool COminiDirectionShadowMap::InitRenderTargetViews(ID3D11Device* pD3DDevice)
	{
		//レンダリングターゲットビューを作成する。
		D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
		ZeroMemory(&rtDesc, sizeof(rtDesc));
		rtDesc.Format = m_texDesc.Format;
		
		for (int i = 0; i < NUM_TEXTURE; i++) {
			rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtDesc.Texture2DArray.FirstArraySlice = i;
			rtDesc.Texture2DArray.ArraySize = 1;
			auto hr = pD3DDevice->CreateRenderTargetView(
				m_shadowMap, &rtDesc, &m_renderTargets[i]);
			if (FAILED(hr)) {
				//レンダリングターゲットビューの作成に失敗した。
				return false;
			}
		}
		return true;
	}
	bool COminiDirectionShadowMap::InitDepthStencilTextures(ID3D11Device* pD3DDevice)
	{
		m_depthTexDesc = m_texDesc;
		m_depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		auto hr = pD3DDevice->CreateTexture2D(&m_depthTexDesc, nullptr, &m_depthStencilTexture);
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}
	bool COminiDirectionShadowMap::InitDepthStencilViews(ID3D11Device* pD3DDevice)
	{
		CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(
			D3D11_DSV_DIMENSION_TEXTURE2D,
			m_depthTexDesc.Format
		);

		auto hr = pD3DDevice->CreateDepthStencilView(
			m_depthStencilTexture, &descDSV, &m_depthStencilView
		);
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}
	bool COminiDirectionShadowMap::Init(const SOminiShadowRenderConfig& config)
	{
		if (config.isEnable == false) {
			return true;
		}

		auto pD3DDevice = GraphicsEngine().GetD3DDevice();
		
		//シャドウマップ描画用のテクスチャを作成する。
		if (InitShadowMapTextures(pD3DDevice) == false) {
			//シャドウマップ用のテクスチャの作成に失敗。
			TK_WARNING_MESSAGE_BOX("COminiDirectionShadowMap::InitShadowMapTextures()で失敗しました。");
			return false;
		}
		//レンダリングターゲットビューを作成。
		if (InitRenderTargetViews(pD3DDevice) == false) {
			//レンダリングターゲットビューの作成に失敗。
			TK_WARNING_MESSAGE_BOX("COminiDirectionShadowMap::InitRenderTargetViews()で失敗しました。");
			return false;
		}
		//デプスステンシルテクスチャを作成する。
		if (InitDepthStencilTextures(pD3DDevice) == false) {
			TK_WARNING_MESSAGE_BOX("COminiDirectionShadowMap::InitDepthStencilTextures()で失敗しました。");
			return false;
		}
		//デプスステンシルビューを作成する。
		if (InitDepthStencilViews(pD3DDevice) == false) {
			TK_WARNING_MESSAGE_BOX("COminiDirectionShadowMap::InitDepthStencilViews()で失敗しました。");
			return false;
		}

		//ＧＰＵ側の定数バッファを作成する。
		m_ominiDirectionShadowCbGPU.Create(nullptr, sizeof(SOminiDirectionShadowCb));

		m_isInited = config.isEnable;
		m_isEnable = true;
		return true;
	}

	void COminiDirectionShadowMap::Update()
	{
		
	}
	
	void COminiDirectionShadowMap::RenderToShadowMapImp(CRenderContext& rc)
	{
		rc.SetRenderStep(enRenderStep_RenderToShadowMap);

		BeginGPUEvent(L"enRenderStep_COminiDirectionShadowMap::RenderToShadowMap");

		if (IsEnable() == false) {
			return;
		}
		static const CVector3 cameraDirectionTbl[NUM_TEXTURE] = {
			{ 1.0f,  0.0f,  0.0f },	//POSITIVE_X
			{-1.0f,  0.0f,  0.0f },	//NEGATIVE_X
			{ 0.0f,  1.0f,  0.0f },	//POSITIVE_Y
			{ 0.0f, -1.0f,  0.0f },	//NEGATIVE_Y
			{ 0.0f,  0.0f,  1.0f },	//POSITIVE_Z
			{ 0.0f,  0.0f, -1.0f },	//NEGATIVE_Z
		};
		static const CVector3 cameraUpTbl[NUM_TEXTURE] = {
			{ 0.0f,  1.0f,  0.0f },	//POSITIVE_X
			{ 0.0f,  1.0f,  0.0f },	//NEGATIVE_X
			{ 0.0f,  0.0f, -1.0f },	//POSITIVE_Y
			{ 0.0f,  0.0f,  1.0f },	//NEGATIVE_Y
			{ 0.0f,  1.0f,  0.0f },	//POSITIVE_Z
			{ 0.0f,  1.0f,  0.0f },	//NEGATIVE_Z
		};

		//全方位シャドウマップをレジスタから外す。
		rc.PSUnsetShaderResource(enSkinModelSRVReg_OminiDirectionShadowMap);

		CRenderTarget* oldRenderTargets[MRT_MAX];
		unsigned int numRenderTargetViews;
		rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);

		for (int i = 0; i < NUM_TEXTURE; i++) {
			BeginGPUEvent(L"Render 3DModel");
			//カメラ行列を作成する。
			CMatrix mView;
			mView.MakeLookAt(
				m_ominiDirectionShadowCbCPU.lightPosition,
				m_ominiDirectionShadowCbCPU.lightPosition + cameraDirectionTbl[i],
				cameraUpTbl[i] );
			//射影行列を作成する。
			CMatrix mProj;
			mProj.MakeProjectionMatrix(
				CMath::DegToRad(90.0f),
				1.0f,
				m_near,
				m_ominiDirectionShadowCbCPU.distanceAffectedByLight
			);

			m_ominiDirectionShadowCbCPU.lightViewProjMatrix[i] = mView * mProj;


			rc.OMSetRenderTarget(m_renderTargets[i], m_depthStencilView);
			rc.RSSetViewport(
				0.0f, 0.0f,
				(float)m_texDesc.Width,
				(float)m_texDesc.Height
			);
			float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
			rc.ClearRenderTargetView(0, ClearColor);
			//
			//シャドウキャスターをドロー
			for (auto& caster : m_shadowCaster) {
				caster->Render(rc, m_ominiDirectionShadowCbCPU.lightViewProjMatrix[i]);
			}

			EndGPUEvent();
		}

		//レンダリングターゲットを差し戻す。
		rc.OMSetRenderTargets(numRenderTargetViews, oldRenderTargets);
		rc.RSSetViewport(0.0f, 0.0f, (float)GraphicsEngine().GetFrameBufferWidth(), (float)GraphicsEngine().GetFrameBufferHeight());

		EndGPUEvent();
	}
	void COminiDirectionShadowMap::SendShadowReceiveParamToGPU(CRenderContext& rc)
	{
		if (IsEnable() == false) {
			return;
		}
		rc.UpdateSubresource(m_ominiDirectionShadowCbGPU, &m_ominiDirectionShadowCbCPU);
		rc.PSSetConstantBuffer(enSkinModelCBReg_OminiDirectionShadow, m_ominiDirectionShadowCbGPU);
		//キューブマップを転送する。
		rc.PSSetShaderResource(enSkinModelSRVReg_OminiDirectionShadowMap, m_shadowMapSRV);
	}
}