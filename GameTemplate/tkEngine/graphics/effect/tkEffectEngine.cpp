/*!
 * @brief	エフェクトエンジン。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/effect/tkEffectEngine.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

namespace tkEngine {
	CEffectEngine::CEffectEngine()
	{
	}
	CEffectEngine::~CEffectEngine()
	{
		Release();
	}

	void CEffectEngine::Release()
	{
		if (m_manager != nullptr) {
			m_manager->Destroy();
			m_manager = nullptr;
		}
		if (m_renderer != nullptr) {
			m_renderer->Destroy();
			m_renderer = nullptr;
		}
		if (m_finalCombineAddBlendState != nullptr) {
			m_finalCombineAddBlendState->Release();
			m_finalCombineAddBlendState = nullptr;
		}
		m_resourcetManager.Release();
	}
	void CEffectEngine::Init()
	{
		Release();
		//レンダラーを初期化。
		m_renderer = EffekseerRendererDX11::Renderer::Create(
			GraphicsEngine().GetD3DDevice(),
			GraphicsEngine().GetD3DDeviceContext(),
			GraphicsEngine().GetD3DImmediateDeviceContext(),
			2000);
		//エフェクトマネージャを初期化。
		m_manager = Effekseer::Manager::Create(10000);

		// 描画用インスタンスから描画機能を設定
		m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_manager->SetModelLoader(m_renderer->CreateModelLoader());

		CGraphicsEngine& ge = GraphicsEngine();
		//加算もの書き込み用のレンダリングターゲットの作成。
		m_addEffectBuffer.Create(
			ge.GetFrameBufferWidth(),
			ge.GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,	//Zバッファは作らない。
			ge.GetMainRenderTargetMSAADesc()
		);
		//Zバッファはメインレンダリングターゲットのものを使用する。
		m_addEffectBuffer.SetDepthStencilView(
			ge.GetMainRenderTarget().GetDepthStencilView()
		);

		m_renderFlags.resize(1024);
		//コピー用のシェーダーをロード。
		m_copyVS.Load("shader/copy.fx", "VSMain", CShader::EnType::VS);
		m_copyPS.Load("shader/copy.fx", "PSMain", CShader::EnType::PS);

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		ID3D11Device* pd3d = ge.GetD3DDevice();

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_BLUE | D3D11_COLOR_WRITE_ENABLE_GREEN;
		pd3d->CreateBlendState(&blendDesc, &m_finalCombineAddBlendState);

		m_cb.Create(nullptr, 16);

	}
	
	Effekseer::Effect* CEffectEngine::CreateEffekseerEffect(const wchar_t* filePath)
	{
		return Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)filePath);
	}
	Effekseer::Handle CEffectEngine::Play(Effekseer::Effect* effect )
	{
		return m_manager->Play(effect, 0, 0, 0);
	}
	void CEffectEngine::Stop(Effekseer::Handle handle)
	{
		m_manager->StopEffect(handle);
	}
	void CEffectEngine::Update()
	{
		//カメラ行列を設定。
		m_renderer->SetCameraMatrix(MainCamera().GetViewMatrix());
		m_renderer->SetProjectionMatrix(MainCamera().GetProjectionMatrix());

		m_manager->Update(GameTime().GetFrameDeltaTime() / (1.0f / 60.0f));
	}
	void CEffectEngine::Render(CRenderContext& rc, CPostEffect* ps)
	{
		BeginGPUEvent(L"enRenderStep_RenderEffect");
		//レンダリングステップを3Dモデルの描画に。
		rc.SetRenderStep(enRenderStep_RenderEffect);

		//現在のレンダリングターゲットをバックアップする。
		CRenderTarget* oldRenderTargets[MRT_MAX];
		unsigned int numRenderTargetViews;
		rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);

		using namespace Effekseer;
		auto nodeCount = 0;
		m_manager->QueryNode([&](EffectNode* node) {
			EffectBasicRenderParameter param = node->GetBasicRenderParameter();
			m_renderFlags[nodeCount] = node->GetRenderFlag();	//元々の描画フラグをバックアップする。
			nodeCount++;
		});

		{
			//加算物以外は通常レンダリング。
			nodeCount = 0;
			m_manager->QueryNode([&](EffectNode* node) {
				EffectBasicRenderParameter param = node->GetBasicRenderParameter();
				if (param.AlphaBlend == AlphaBlendType::Add) {
					//加算物は描画しない。
					node->SetRenderFlag(false);
				}
			});

			m_renderer->BeginRendering();
			m_manager->Draw();
			m_renderer->EndRendering();
		}
		{
			//続いて加算物。
			nodeCount = 0;
			m_manager->QueryNode([&](EffectNode* node) {
				EffectBasicRenderParameter param = node->GetBasicRenderParameter();
				if (param.AlphaBlend == AlphaBlendType::Add) {
					//加算物の描画フラグを戻す。
					node->SetRenderFlag(m_renderFlags[nodeCount]);
				}
				else {
					//加算物以外は描画しない。
					node->SetRenderFlag(false);
				}
				nodeCount++;
			});

			//加算物はオフスクリーンレンダリングを行って、後で合成する。
			CRenderTarget* renderTargets[] = {
				&m_addEffectBuffer
			};
			rc.OMSetRenderTargets(1, renderTargets);
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; //red,green,blue,alpha
			rc.ClearRenderTargetView(0, ClearColor, false);

			//レンダリングステップを3Dモデルの描画に。

			rc.SetRenderStep(enRenderStep_RenderEffect);

			//加算物を描画する。
			m_renderer->BeginRendering();
			m_manager->Draw();
			m_renderer->EndRendering();

			//レンダリングターゲットを差し戻す。
			rc.OMSetRenderTargets(numRenderTargetViews, oldRenderTargets);
			//MSAAリゾルブ。
			m_addEffectBuffer.ResovleMSAATexture(rc);

			CVector2 uvOffset;
			uvOffset.x = 0.5f / oldRenderTargets[0]->GetWidth();
			uvOffset.y = 0.5f / oldRenderTargets[0]->GetHeight();
			rc.UpdateSubresource(m_cb, &uvOffset);
			rc.PSSetConstantBuffer(0, m_cb);
			rc.PSSetShaderResource(0, m_addEffectBuffer.GetRenderTargetSRV());
			rc.PSSetShader(m_copyPS);
			rc.VSSetShader(m_copyVS);
		
			ID3D11DepthStencilState* oldDepthStencil = rc.GetDepthStencilState();
			ID3D11RasterizerState* oldRSState = rc.GetRSState();
			ID3D11BlendState* oldBlendState = rc.GetBlendState();


			rc.RSSetState(RasterizerState::spriteRender);
			rc.OMSetDepthStencilState(DepthStencilState::disable);
			rc.OMSetBlendState(m_finalCombineAddBlendState );

			ps->DrawFullScreenQuad(rc);

			//戻す。
			rc.OMSetDepthStencilState(oldDepthStencil);
			rc.RSSetState(oldRSState);
			rc.OMSetBlendState(oldBlendState);

		}

		//ノードの描画フラグを全部戻す。
		nodeCount = 0;
		m_manager->QueryNode([&](EffectNode* node) {
			node->SetRenderFlag(m_renderFlags[nodeCount]);
			nodeCount++;
		});


		EndGPUEvent();
	}
}