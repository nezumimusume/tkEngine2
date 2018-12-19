/// <summary>
/// DOF(被写界深度
/// </summary>
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkDof.h"
#include "tkEngine/graphics/tkPresetRenderState.h"

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
		m_cocAndColorRt.Release();
		m_applyBinFilterAndCalcNearCocVS.Release();
		m_applyBinFilterAndCalcNearCocPS.Release();
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
		
	}
	void CDof::InitShaders()
	{
		m_applyBinFilterAndCalcNearCocVS.Load(
			"shader/dof/dof_ApplyBinFilterAndCalcNearCoC.fx",
			"VSMain",
			CShader::EnType::VS);
		m_applyBinFilterAndCalcNearCocPS.Load(
			"shader/dof/dof_ApplyBinFilterAndCalcNearCoC.fx",
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
		//1/4の解像度。ダウンサンプリングを行うよ。
		m_cocAndColorRt.Create(
			ge.GetFrameBufferWidth() / 2,
			ge.GetFrameBufferHeight() / 2,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			multiSampleDesc
		);
	}
	void CDof::Update()
	{
	}
	void CDof::ApplyBilinearFilterAndCalcNearCoc(CRenderContext& rc, CPostEffect* postEffect)
	{
		auto& ge = GraphicsEngine();
		ge.BeginGPUEvent(L"enRenderStep_Dof::ApplyBilinearFilterAndCalcNearCoc");
		//シーンが書き込まれているレンダリングターゲットを取得する。
		auto& sceneRt = postEffect->GetFinalRenderTarget();
		//レンダリングターゲットを切り替える。
		CRenderTarget* rts[] = {
			&m_cocAndColorRt
		};
		rc.OMSetRenderTargets(1, rts );
		rc.PSSetShaderResource(0, sceneRt.GetRenderTargetSRV());
		rc.VSSetShader(m_applyBinFilterAndCalcNearCocVS);
		rc.IASetInputLayout(m_applyBinFilterAndCalcNearCocVS.GetInputLayout());
		rc.PSSetShader(m_applyBinFilterAndCalcNearCocPS);
		rc.RSSetViewport(0, 0, m_cocAndColorRt.GetWidth(), m_cocAndColorRt.GetHeight());
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
		rc.SetRenderStep(enRenderStep_Dof);
		//シーンテクスチャにバイリニアフィルタをかけて、
		//カメラに近い側のCoC(錯乱円の半径)の計算を行います。
		ApplyBilinearFilterAndCalcNearCoc(rc, postEffect);

		ge.EndGPUEvent();
	}
}