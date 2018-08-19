#include "stdafx.h"
#include "GameOverCamera.h"
#include "Player.h"
#include "Enemy.h"

GameOverCamera::GameOverCamera()
{
}


GameOverCamera::~GameOverCamera()
{
}
void GameOverCamera::Init(const Player& player, const Enemy& enemy)
{
	m_player = &player;
	m_enemy = &enemy;
}
bool GameOverCamera::Start()
{
	m_springCamera.Init(MainCamera(), 800.0f, false, 0.0f);
	m_springCamera.Update();
	CVector3 target = m_player->GetPosition() + m_enemy->GetPosition();
	target *= 0.5f;
	m_springCamera.SetTarget(target);
	CVector3 position = target;
	//^‰¡‚É—ˆ‚é‚æ‚¤‚ÈƒJƒƒ‰‚É‚µ‚Ä‚Ý‚é‚ñ‚²B
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 45.0f);
	CVector3 toPos = m_player->GetPosition() - m_enemy->GetPosition();
	toPos.y = 0.0f;
	toPos.Normalize();
	qRot.Apply(toPos);

	toPos *= 400.0f;
	CVector3 pos = target + toPos;
	pos.y += 200.0f;
	m_springCamera.SetPosition(pos);
	return true;
}

void GameOverCamera::Update()
{
	m_springCamera.Update();
}
