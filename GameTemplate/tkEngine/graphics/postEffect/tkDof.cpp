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
		m_createDofMaskAndCalcCocParam.calcCocAndColorRt.Release();
		m_createDofMaskAndCalcCocParam.vs.Release();
		m_createDofMaskAndCalcCocParam.ps.Release();
		m_createDofMaskAndCalcCocParam.cb.Release();
		if (m_createDofMaskAndCalcCocParam.blendState != nullptr) {
			m_createDofMaskAndCalcCocParam.blendState->Release();
		}

		m_downSampligCocAndColorParam.vs.Release();
		m_downSampligCocAndColorParam.ps.Release();
		
	}
	void CDof::Init(const SGraphicsConfig& config)
	{
		if (config.dofConfig.isEnable==false) {
			return;
		}
		m_isEnable = config.dofConfig.isEnable;
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
		m_createDofMaskAndCalcCocParam.cb.Create(nullptr, sizeof(SCreateDofMaskAndCalcCoc));
		m_downSampligCocAndColorParam.cb.Create(nullptr, sizeof(SDownSamplingCocAndColorCB));
	}

	void CDof::InitShaders()
	{
		m_createDofMaskAndCalcCocParam.vs.Load(
			"shader/dof/dof_CreateDofMaskAndCalcCoc.fx",
			"VSMain",
			CShader::EnType::VS);
		m_createDofMaskAndCalcCocParam.ps.Load(
			"shader/dof/dof_CreateDofMaskAndCalcCoc.fx",
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
		m_createDofMaskAndCalcCocParam.calcCocAndColorRt.Create(
			ge.GetFrameBufferWidth(),
			ge.GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
		//ガウシアーン。
		m_downSampligCocAndColorParam.blur[0].Init(
			m_createDofMaskAndCalcCocParam.calcCocAndColorRt.GetRenderTargetSRV(),
			2.5f
		);
		m_downSampligCocAndColorParam.blur[1].Init(
			m_downSampligCocAndColorParam.blur[0].GetResultSRV(),
			2.5f,
			true
		);
		/*m_downSampligCocAndColorParam.blur[2].Init(
			m_downSampligCocAndColorParam.blur[1].GetResultSRV(),
			2.5f,
			true
		);*/
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

		d3dDevice->CreateBlendState(&desc, &m_createDofMaskAndCalcCocParam.blendState);
	}
	void CDof::Update()
	{
	}
	void CDof::CreateDofMaskAndCalcCoc(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof::CreateDofMaskAndCalcCoc");
		//シーンが書き込まれているレンダリングターゲットを取得する。
		auto& sceneRt = postEffect->GetFinalRenderTarget();
		auto& calcCocAndColorRt = m_createDofMaskAndCalcCocParam.calcCocAndColorRt;
		auto& vs = m_createDofMaskAndCalcCocParam.vs;
		auto& ps = m_createDofMaskAndCalcCocParam.ps;
		auto& cb = m_createDofMaskAndCalcCocParam.cb;
		auto& depthTextureSrv = ge.GetGBufferRender().GetRenderTarget(enGBufferDepth).GetRenderTargetSRV();
		//レンダリングターゲットを切り替える。

		CChangeRenderTarget chgRt(rc, calcCocAndColorRt);
		//定数バッファの更新。
		SCreateDofMaskAndCalcCocCB cbParam;
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
		rc.OMSetBlendState(m_createDofMaskAndCalcCocParam.blendState);
		
		postEffect->DrawFullScreenQuad(rc);

		//レンダリングターゲットを外す。
		rc.OMSetRenderTargets(2, nullptr);

		ge.EndGPUEvent();

	}
	void CDof::DownSamplingCocAndColor(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		auto& depthTextureSrv = ge.GetGBufferRender().GetRenderTarget(enGBufferDepth).GetRenderTargetSRV();

		ge.BeginGPUEvent(L"enRenderStep_Dof::DownSamplingCocAndColor");

		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.PSSetShaderResource(1, depthTextureSrv);
		for (auto& blur : m_downSampligCocAndColorParam.blur) {
			blur.Execute(rc);
		}

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
		rc.PSSetShaderResource(0, m_createDofMaskAndCalcCocParam.calcCocAndColorRt.GetRenderTargetSRV());
		rc.PSSetShaderResource(1, m_downSampligCocAndColorParam.blur[0].GetResultSRV());
		rc.PSSetShaderResource(2, m_downSampligCocAndColorParam.blur[1].GetResultSRV());
	
		rc.OMSetBlendState(AlphaBlendState::disable);
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		rc.PSSetSampler(1, *CPresetSamplerState::clamp_clamp_clamp_point);
		postEffect->DrawFullScreenQuad(rc);

		ge.EndGPUEvent();
	}
	void CDof::Render(CRenderContext& rc, CPostEffect* postEffect)
	{
		if (m_isEnable == false) {
			return;
		}
		
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof");

		//レンダリングステートを退避する。
		rc.PushRenderState();

		rc.SetRenderStep(enRenderStep_Dof);

		CreateDofMaskAndCalcCoc(rc, postEffect);

		DownSamplingCocAndColor(rc, postEffect);

		Final(rc, postEffect);

		//レンダリングステートを戻す。
		rc.PopRenderState(true);

		ge.EndGPUEvent();
	}
}