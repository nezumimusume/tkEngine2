/// <summary>
/// DOF(被写界深度
/// </summary>
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkDof.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkPresetSamplerState.h"

namespace tkEngine {
	CDof::CDof()
	{
	}
	CDof::~CDof()
	{
		Release();
	}
	void CDof::Release()
	{
		m_createCocParam.calcCocAndColorRt.Release();
		m_createCocParam.vs.Release();
		m_createCocParam.ps.Release();
		m_createCocParam.cb.Release();
		if (m_createCocParam.blendState != nullptr) {
			m_createCocParam.blendState->Release();
		}

		m_createBokeTextureParam.vs.Release();
		m_createBokeTextureParam.ps.Release();
		m_createBokeTextureParam.cb.Release();
	}
	void CDof::Init(const SGraphicsConfig& config)
	{
		if (config.dofConfig.isEnable==false) {
			return;
		}
		m_isEnable = config.dofConfig.isEnable;
		m_isEnableGlobal = config.dofConfig.isEnable;
		//解放。
		Release();

		//シェーダーを初期化。
		InitShaders();
		//レンダリングターゲットを初期化。
		InitRenderTargets();
		//定数バッファの初期化。
		InitConstantBuffers();
	}
	void CDof::InitConstantBuffers()
	{
		m_createCocParam.cb.Create(nullptr, sizeof(SCreateCocParamCB));
		m_createBokeTextureParam.cb.Create(nullptr, sizeof(SCreateBokeTextureCB));
		m_finalParam.gpuCB.Create(nullptr, sizeof(SFinalCB));
	}

	void CDof::InitShaders()
	{
		m_createCocParam.vs.Load(
			"shader/dof/dof_CreateCoCTexture.fx",
			"VSMain",
			CShader::EnType::VS);
		m_createCocParam.ps.Load(
			"shader/dof/dof_CreateCoCTexture.fx",
			"PSMain",
			CShader::EnType::PS);

		m_finalParam.vs.Load(
			"shader/dof/dof_Final.fx",
			"VSMain",
			CShader::EnType::VS);
		m_finalParam.ps.Load(
			"shader/dof/dof_Final.fx",
			"PSMain",
			CShader::EnType::PS);
	}
	void CDof::InitRenderTargets()
	{
		auto& ge = GraphicsEngine();
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		//CoC計算用のレンダリングターゲットを作成する。
		m_createCocParam.calcCocAndColorRt.Create(
			ge.GetFrameBufferWidth(),
			ge.GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		m_createBokeTextureParam.blur.Init(
			m_createCocParam.calcCocAndColorRt.GetRenderTargetSRV(),
			1.5f
		);
	}
	void CDof::InitBlendStates()
	{
		auto d3dDevice = GraphicsEngine().GetD3DDevice();
		CD3D11_DEFAULT def;
		CD3D11_BLEND_DESC desc(def);

		//CoCを計算するレンダリングターゲットはアルファブレンディングはオフ。
		desc.RenderTarget[0].BlendEnable = false;
		//Dofマスクを作成するレンダリングターゲットもアルファブレンディングはオフ。
		desc.RenderTarget[1].BlendEnable = false;

		d3dDevice->CreateBlendState(&desc, &m_createCocParam.blendState);
	}
	void CDof::Update()
	{
	}
	void CDof::CreateCoCTexture(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof::CreateCoCTexture");
		//シーンが書き込まれているレンダリングターゲットを取得する。
		auto& sceneRt = postEffect->GetFinalRenderTarget();
		auto& calcCocAndColorRt = m_createCocParam.calcCocAndColorRt;
		auto& vs = m_createCocParam.vs;
		auto& ps = m_createCocParam.ps;
		auto& cb = m_createCocParam.cb;
		auto& depthTextureSrv = ge.GetGBufferRender().GetRenderTarget(enGBufferDepth).GetRenderTargetSRV();
		//レンダリングターゲットを切り替える。

		CChangeRenderTarget chgRt(rc, calcCocAndColorRt);
		//定数バッファの更新。
		SCreateCocParamCB cbParam;
		cbParam.dofRange.x = m_nearStartDistance;
		cbParam.dofRange.y = m_nearEndDistance;
		cbParam.dofRange.z = m_farStartDistance;
		cbParam.dofRange.w = m_farEndDistance;

		//メインメモリからVRAMにデータを転送。
		rc.UpdateSubresource(cb, &cbParam);
		//定数バッファをレジスタb0に設定する。
		rc.VSSetConstantBuffer(0, cb);
		rc.PSSetConstantBuffer(0, cb);
		rc.PSSetShaderResource(0, sceneRt.GetRenderTargetSRV());
		rc.PSSetShaderResource(1, depthTextureSrv);
		rc.VSSetShader(vs);
		rc.PSSetShader(ps);

		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_point);
		rc.OMSetBlendState(m_createCocParam.blendState);
		
		postEffect->DrawFullScreenQuad(rc);

		//レンダリングターゲットを外す。
		rc.OMSetRenderTargets(2, nullptr);

		ge.EndGPUEvent();

	}
	void CDof::CreateBokeTexture(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		auto& depthTextureSrv = ge.GetGBufferRender().GetRenderTarget(enGBufferDepth).GetRenderTargetSRV();

		ge.BeginGPUEvent(L"enRenderStep_Dof::CreateBokeTexture");

		//ボケ画像の生成
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.PSSetShaderResource(1, depthTextureSrv);
		m_createBokeTextureParam.blur.Execute(rc);
		

		ge.EndGPUEvent();
	}
	void CDof::Final(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof::Final");

		//レンダリングターゲット切り替え。
		CChangeRenderTarget chgRt(rc, postEffect->GetFinalRenderTarget());

		rc.VSSetShader(m_finalParam.vs);
		rc.PSSetShader(m_finalParam.ps);
		rc.PSSetShaderResource(0, m_createCocParam.calcCocAndColorRt.GetRenderTargetSRV());
		rc.PSSetShaderResource(1, m_createBokeTextureParam.blur.GetHexaBlurResultSRV());
	
		rc.OMSetBlendState(AlphaBlendState::disable);
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.PSSetSampler(1, *CPresetSamplerState::clamp_clamp_clamp_point);
		rc.UpdateSubresource(m_finalParam.gpuCB, &m_finalParam.cpuCB);
		rc.PSSetConstantBuffer(0, m_finalParam.gpuCB);

		postEffect->DrawFullScreenQuad(rc);

		ge.EndGPUEvent();
	}
	void CDof::Render(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (m_isEnable == false || m_isEnableGlobal == false) {
			return;
		}
		
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof");

		//レンダリングステートを退避する。
		rc.PushRenderState();

		rc.SetRenderStep(enRenderStep_Dof);

		CreateCoCTexture(rc, postEffect);

		CreateBokeTexture(rc, postEffect);

		Final(rc, postEffect);

		//レンダリングステートを戻す。
		rc.PopRenderState(true);

		ge.EndGPUEvent();
	}
}