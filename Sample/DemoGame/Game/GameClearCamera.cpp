#include "stdafx.h"
#include "GameClearCamera.h"
#include "Player.h"

GameClearCamera::GameClearCamera()
{
}


GameClearCamera::~GameClearCamera()
{
}

bool GameClearCamera::Start()
{
	m_player = FindGO<Player>("Player");
	m_springCamera.Init(MainCamera(), 800.0f, false, 0.0f);
	m_springCamera.Update();

	CVector3 target = m_player->GetPosition();
	target.y += 50.0f;
	CVector3 pos = target + m_player->GetForward() * 200.0f;
	pos.y += 100.0f;
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(pos);
	return true;
}
void GameClearCamera::Update()
{
	m_springCamera.Update();
}