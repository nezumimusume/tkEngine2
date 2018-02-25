/*!
 * @brief	背景。
 */
#include "stdafx.h"
#include "Background.h"
#include "tkEngine/graphics/tkPresetRenderState.h"


bool Background::Start()
{

	m_skinModelData.Load(L"modelData/Background.cmo");

	m_skinModel.Init(m_skinModelData);
	m_skinModel.SetShadowReceiverFlag(true);
	m_skinModel.SetShadowCasterFlag(true);

	m_meshCollider.CreateFromSkinModel(m_skinModel, nullptr);
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


	return true;
}
void Background::OnDestroy()
{
	PhysicsWorld().RemoveRigidBody(m_rigidBody);
	if (m_rasterizerState) {
		m_rasterizerState->Release();
	}
}
void Background::Update()
{
	m_skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void Background::Render(CRenderContext& rc)
{
	rc.RSSetState(m_rasterizerState);
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
	rc.RSSetState(RasterizerState::sceneRender);
}