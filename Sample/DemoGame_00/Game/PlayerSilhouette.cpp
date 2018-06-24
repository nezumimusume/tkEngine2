#include "stdafx.h"
#include "PlayerSilhouette.h"
#include "Player.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

PlayerSilhouette::PlayerSilhouette()
{
}


PlayerSilhouette::~PlayerSilhouette()
{
	if (m_depthStencilState) {
		m_depthStencilState->Release();
	}
}
bool PlayerSilhouette::Start()
{
	m_player = FindGO<Player>("Player");
	
	//深度ステンシルステートを作成。
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ID3D11Device* pd3d = GraphicsEngine().GetD3DDevice();
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	desc.StencilEnable = false;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	pd3d->CreateDepthStencilState(&desc, &m_depthStencilState);
	m_psShader.Load("shader/model.fx", "PSMain_Silhouette", CShader::EnType::PS);
	m_texture.CreateFromDDSTextureFromFile(L"modelData/UnityChanLogo.dds");
	return true;
}
void PlayerSilhouette::Update()
{
}
void PlayerSilhouette::ForwardRender(CRenderContext& rc)
{
	return;
	auto model = &m_player->GetModel();
	rc.OMSetDepthStencilState(m_depthStencilState, 0);
	//シルエット描画用のシェーダーに差し替える。
	model->FindMaterial([&](CModelEffect* effect) {
		effect->SetRender3DModelPSShader(m_psShader);
	});
	rc.PSSetShaderResource(enSkinModelSRVReg_SilhouetteTexture, m_texture);
	model->Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
	rc.OMSetDepthStencilState(DepthStencilState::SceneRender, 0);
	//
	model->FindMaterial([&](CModelEffect* effect) {
		effect->SetRender3DModelDefaultShader();
	});
}
