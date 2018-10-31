#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	m_skinModelRender->SetShadowCasterFlag(true);
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 180.0f);
	m_skinModelRender->SetRotation(qRot);
	m_charaCon.Init(30.0f, 60.0f, m_position);
}


Player::~Player()
{
	DeleteGO(m_skinModelRender);
}

void Player::Update()
{
	CPad& pad = Pad(0);
	m_moveSpeed.x = pad.GetLStickXF() * 400.0f;
	m_moveSpeed.z = pad.GetLStickYF() * 400.0f;

	m_position = m_charaCon.Execute(m_moveSpeed);

	m_skinModelRender->SetPosition(m_position);
}