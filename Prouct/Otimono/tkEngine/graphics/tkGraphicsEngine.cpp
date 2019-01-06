/*!
 *@brief	描画エンジン。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkPresetSamplerState.h"

namespace tkEngine{
	CGraphicsEngine::CGraphicsEngine()
	{
	}
	CGraphicsEngine::~CGraphicsEngine()
	{
	}
	void CGraphicsEngine::Release()
	{

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		if (m_userAnnoation) {
			m_userAnnoation->Release();
			m_userAnnoation = nullptr;
		}
#endif
		if (m_backBufferRT) {
			m_backBufferRT->Release();
			m_backBufferRT = nullptr;
		}
		if (m_pImmediateContext) {
			m_pImmediateContext->ClearState();
			m_pImmediateContext = nullptr;
		}
		m_mainRenderTarget.Release();
		
		if (m_pSwapChain) {
			m_pSwapChain->Release();
			m_pSwapChain = nullptr;
		}
		for (auto& commandList : m_commandList) {
			if (commandList != nullptr) {
				commandList->Release();
				commandList = nullptr;
			}
		}
		if (m_pImmediateContext) {
			m_pImmediateContext->Release();
			m_pImmediateContext = nullptr;
		}
		if (m_pDeferredDeviceContext) {
			m_pDeferredDeviceContext->Release();
			m_pDeferredDeviceContext = nullptr;
		}
		if (m_pd3dDevice) {
			m_pd3dDevice->Release();
			m_pd3dDevice = nullptr;
		}
	}
	/*!
	*@brief	ディファードシェーディングの初期化。
	*/
	void CGraphicsEngine::InitDefferdShading()
	{
		//ディファードシェーディング用のシェーダーをロード。
		m_vsDefferd.Load("shader/defferdShading.fx", "VSMain", CShader::EnType::VS);
		m_psDefferd.Load("shader/defferdShading.fx", "PSMain", CShader::EnType::PS);
		//定数バッファを初期化。
		m_cbDefferd.Create(nullptr, sizeof(PSDefferdCb));
	}
	bool CGraphicsEngine::InitD3DDeviceAndSwapChain(HWND hwnd, const SInitParam& initParam)
	{
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		m_frameBufferWidth = initParam.frameBufferWidth;
		m_frameBufferHeight = initParam.frameBufferHeight;
		
		//スワップチェインを作成。
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;							//スワップチェインのバッファ数。通常は１。
		sd.BufferDesc.Width = m_frameBufferWidth;	//フレームバッファの幅。
		sd.BufferDesc.Height = m_frameBufferHeight;	//フレームバッファの高さ。
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フレームバッファのフォーマット。R8G8B8A8の32bit。
		sd.BufferDesc.RefreshRate.Numerator = 60;			//モニタのリフレッシュレート。(バックバッファとフロントバッファを入れ替えるタイミングとなる。)
		sd.BufferDesc.RefreshRate.Denominator = 1;			//２にしたら30fpsになる。あとで試す。
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;							//ピクセル単位のマルチサンプリングの数。MSAAはなし。
		sd.SampleDesc.Quality = 0;							//MSAAなし。
		sd.Windowed = TRUE;

		//すべてのドライバタイプでスワップチェインの作成を試す。
		HRESULT hr = E_FAIL;
		for (auto driverType : driverTypes) {
			m_driverType = driverType;
			hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
			if (SUCCEEDED(hr)) {
				//スワップチェインを作成できたのでループを抜ける。
				break;
			}
		}
		if (FAILED(hr)) {
			//スワップチェインを作成できなかった。
			return false;
		}
		//デバイスのマルチスレッドサポートの機能を調べる。
		m_pd3dDevice->CheckFeatureSupport(
			D3D11_FEATURE_THREADING,
			&m_featureDataThreading,
			sizeof(m_featureDataThreading)
			);
		
		
		if (m_featureDataThreading.DriverCommandLists == TRUE) {
			//デバイズがディファードコンテキストに対応しているので
			//ディファードコンテキストを作成。
			m_pd3dDevice->CreateDeferredContext(0, &m_pDeferredDeviceContext);
		}
		return true;
	}
	
	bool CGraphicsEngine::InitBackBuffer()
	{
		//書き込み先になるレンダリングターゲットを作成。
		ID3D11Texture2D* pBackBuffer = NULL;
		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr)) {
			return false;
		}
		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBufferRT);
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}
	bool CGraphicsEngine::InitMainRenderTarget()
	{
		//MSAAの設定をする。	
		ZeroMemory(&m_mainRenderTargetMSAADesc, sizeof(m_mainRenderTargetMSAADesc));
		m_mainRenderTargetMSAADesc.Count = 1;
		m_mainRenderTargetMSAADesc.Quality = 0;

		bool ret = m_mainRenderTarget.Create(
			m_frameBufferWidth,
			m_frameBufferHeight,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_mainRenderTargetMSAADesc,
			nullptr,
			nullptr,
			true
		);
		
		if (!ret) {
			//作成失敗
			return false;
		}
		return true;
	}
	
	bool CGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
		//D3Dデバイスとスワップチェインの作成。
		if (!InitD3DDeviceAndSwapChain(hwnd, initParam)) {
			return false;
		}
		//バックバッファの作成。
		if (!InitBackBuffer()) {
			return false;
		}
		//メインレンダリングターゲットの初期化。
		if (!InitMainRenderTarget()) {
			return false;
		}
		//レンダリングコンテキストの初期化。
		m_renderContext.Init(m_pImmediateContext, m_pDeferredDeviceContext);
		auto deviceContext = m_renderContext.GetD3DDeviceContext();

		CRenderTarget* renderTargets[] = {
			&m_mainRenderTarget
		};
		m_renderContext.OMSetRenderTargets(1, renderTargets);
		//ビューポートを設定。
		m_renderContext.RSSetViewport(0.0f, 0.0f, (FLOAT)m_frameBufferWidth, (FLOAT)m_frameBufferHeight);
		//PreRenderの初期化。
		m_preRender.Create(initParam.graphicsConfing);
		//PostEffectの初期化。
		m_postEffect.Create(initParam.graphicsConfing);
		//ライト管理者の初期化。
		m_lightManager.Init();

		//コピー用のシェーダーをロード。
		m_copyVS.Load("shader/copy.fx", "VSMain", CShader::EnType::VS);
		m_copyPS.Load("shader/copy.fx", "PSMain", CShader::EnType::PS);

		m_cb.Create(nullptr, 16);
		//ディファードシェーディング用の初期化を行う。
		InitDefferdShading();

		//フォント用のデータの初期化。
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
		m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"font/myfile.spritefont");

		//2Dカメラの初期化。
		m_2dCamera.SetTarget(CVector3::Zero);
		m_2dCamera.SetPosition({0.0f, 0.0f, -10.0f});
		m_2dCamera.SetUpdateProjMatrixFunc(CCamera::enUpdateProjMatrixFunc_Ortho);
		m_2dCamera.SetNear(0.1f);
		m_2dCamera.SetFar(1000.0f);
		m_2dCamera.Update();
		//各種レンダリングステートを初期化する。
		AlphaBlendState::Init(*this);
		DepthStencilState::Init(*this);
		RasterizerState::Init(*this);
		//サンプラステートを初期化する。
		CPresetSamplerState::Init();
		//エフェクトエンジンの初期化。
		m_effectEngine.Init();
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		deviceContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&m_userAnnoation);
#endif
		return true;

	}
	void CGraphicsEngine::BeginRender()
	{
		auto& commandList = m_commandList[m_commandListNoMainThread];
		//作成したコマンドを実行。
		if (commandList != nullptr) {
			m_pImmediateContext->ExecuteCommandList(commandList, FALSE);
			commandList->Release();
			commandList = nullptr;
		}
	}
	void CGraphicsEngine::DefferdShading(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_DefferdShading");
		rc.SetRenderStep(enRenderStep_ForwardRender);
		//ライトの情報を転送転送。
		LightManager().Render(rc);
		//影を落とすための情報を転送。
		GraphicsEngine().GetShadowMap().SendShadowReceiveParamToGPU(rc);
		GraphicsEngine().GetGBufferRender().SetGBufferParamToReg(rc);
		//定数バッファを更新。
		PSDefferdCb cb;
		cb.mViewProjInv.Inverse(MainCamera().GetViewProjectionMatrix());
		rc.UpdateSubresource(m_cbDefferd, &cb);
		//定数バッファをb0のレジスタに設定。
		rc.PSSetConstantBuffer(0, m_cbDefferd);
		//シェーダーを設定。
		rc.VSSetShader(m_vsDefferd);
		rc.PSSetShader(m_psDefferd);

		//ディファードレンダリング用のデプスステンシルステート。
		ID3D11DepthStencilState* depthStencil = rc.GetDepthStencilState();
		//rc.OMSetDepthStencilState(DepthStencilState::defferedRender, 0);
		rc.OMSetDepthStencilState(DepthStencilState::spriteRender);
		//ポストエフェクトのフルスクリーン描画の機能を使う。
		m_postEffect.DrawFullScreenQuad(rc);

		GraphicsEngine().GetGBufferRender().UnsetGBufferParamFromReg(rc);

		rc.OMSetDepthStencilState(depthStencil);

		EndGPUEvent();

	}
	void CGraphicsEngine::EndPostEffect(CRenderContext& rc)
	{
		//バックバッファにメインレンダリングターゲットの内容をコピー。
		ID3D11Texture2D* pBackBuffer = NULL;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		ID3D11RenderTargetView* rts[] = {
			m_backBufferRT
		};
		rc.OMSetRenderTargets(1, rts, nullptr);
		rc.VSSetShader(m_copyVS);
		rc.PSSetShader(m_copyPS);
		CVector2 uvOffset;
		uvOffset.x = 0.5f / m_frameBufferWidth;
		uvOffset.y = 0.5f / m_frameBufferHeight;
		rc.UpdateSubresource(m_cb, &uvOffset);
		rc.PSSetConstantBuffer(0, m_cb);

		rc.PSSetShaderResource(0, m_postEffect.GetFinalRenderTarget().GetRenderTargetSRV());
		rc.RSSetState(RasterizerState::spriteRender);
		rc.RSSetViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);
		//ポストエフェクトのフルスクリーン描画の機能を使う。
		m_postEffect.DrawFullScreenQuad(rc);
		pBackBuffer->Release();
		rc.PSUnsetShaderResource(0);
	}
	void CGraphicsEngine::EndRenderFromGameThread()
	{
		m_lightManager.EndRender(m_renderContext);
		if (IsMultithreadRendering()) {
			//コマンドリストを作成。
			int commandListNo = 1 ^ m_commandListNoMainThread;
			m_pDeferredDeviceContext->FinishCommandList(FALSE, &m_commandList[commandListNo]);
		}
	}

	void CGraphicsEngine::EndRender()
	{
		m_pSwapChain->Present(1, 0);
		if (IsMultithreadRendering()) {
			//コマンドリストを入れ替える。
			m_commandListNoMainThread = 1 ^ m_commandListNoMainThread;
		}
	}
}