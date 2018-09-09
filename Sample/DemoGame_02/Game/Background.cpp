#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/bg/bg.cmo");
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);
	//スペキュラマップを設定。
	m_specTex.CreateFromDDSTextureFromFile(L"modelData/bg/bgSpec.dds");
	m_modelRender->FindMaterial([&](CModelEffect* mat) {
		if (mat->EqualMaterialName(L"floor")) {
			mat->SetSpecularMap(m_specTex.GetBody());
		}
	});
	m_phyStaticObj.CreateMesh(
		CVector3::Zero,
		CQuaternion::Identity,
		CVector3::One,
		m_modelRender
	);
}
Background::~Background()
{
	DeleteGO(m_modelRender);
}
void Background::Update()
{
	//パッドの入力で背景を回転させる。
	CQuaternion qTargetRot, qAddRot;
	qTargetRot.SetRotationDeg(CVector3::AxisX, -30.0f * Pad(0).GetLStickYF());
	qAddRot.SetRotationDeg(CVector3::AxisZ, 30.0f * Pad(0).GetLStickXF());
	qTargetRot *= qAddRot;
	m_rotation.Slerp(0.025f, m_rotation, qTargetRot);
	m_phyStaticObj.SetPositionAndRotation(
		CVector3::Zero,
		m_rotation
	);
	
	m_modelRender->SetRotation(m_rotation);
}
