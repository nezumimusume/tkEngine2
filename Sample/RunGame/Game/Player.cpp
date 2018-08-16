#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	m_charaCon.Init(30, 100, m_position);
}
Player::~Player()
{
	DeleteGO(m_skinModelRender);
}
void Player::Update()
{
	m_moveSpeed.x = Pad(0).GetLStickXF() * 200.0f;
	m_moveSpeed.y -= 20.0f;
	m_moveSpeed.z = Pad(0).GetLStickYF() * 200.0f;
	if (Pad(0).IsTrigger(enButtonA)) {
		m_moveSpeed.y = 400.0f;
	}
	m_position = m_charaCon.Execute(m_moveSpeed);
	m_skinModelRender->SetPosition(m_position);
}
