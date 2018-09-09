#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/bg/bg.cmo");
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);
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
	//ƒpƒbƒh‚Ì“ü—Í‚Å”wŒi‚ð‰ñ“]‚³‚¹‚éB
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
