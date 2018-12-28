/*!
 * @brief	背景。
 */
#include "stdafx.h"
#include "Background.h"
#include "tkEngine/graphics/tkPresetRenderState.h"


bool Background::Start()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/Background.cmo");
	m_modelRender->SetShadowReceiverFlag(true);
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetPreDrawFookFunction([&](auto& renderContext, auto& model) {
		//背面描画にする。
		(void)model;
		renderContext.RSSetState(m_rasterizerState);
	});
	m_modelRender->SetPostDrawFookFunction([&](auto& renderContext, auto& model) {
		//戻す。
		(void)model;
		renderContext.RSSetState(RasterizerState::sceneRender);
	});
	m_meshCollider.CreateFromSkinModel(m_modelRender->GetSkinModel(), nullptr);
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider;

	m_rigidBody.Create(rbInfo);
	PhysicsWorld().AddRigidBody(m_rigidBody);

	//背景描画用のラスタライザステートを作る。
	D3D11_RASTERIZER_DESC desc = {};
	ID3D11Device* pd3d = GraphicsEngine().GetD3DDevice();
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	pd3d->CreateRasterizerState(&desc, &m_rasterizerState);

	//SSRはアルゴリズムを見直す！
/*	m_specMap.CreateFromDDSTextureFromFile(L"modelData/Park-Texture_specMap.dds");
	m_modelRender->FindMaterial([&](CModelEffect* mat) {
		mat->SetSpecularMap(m_specMap.GetBody());
	});*/
	return true;
}
void Background::OnDestroy()
{
	PhysicsWorld().RemoveRigidBody(m_rigidBody);
	if (m_rasterizerState) {
		m_rasterizerState->Release();
	}
	DeleteGO(m_modelRender);
}
