#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "GameObjectName.h"

GameCamera::GameCamera()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(100.0f);
	MainCamera().SetFar(20000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, -200.0f });
	m_springCamera.Init(MainCamera(), 1000.0f, false, 0.0f);

	m_player = FindGO<Player>(PLAYER_NAME);
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	auto target = m_player->GetPosition();
	target.y += 200.0f;
	m_springCamera.SetTarget(target);
	auto position = target;
	position.z -= 800.0f;
	position.y += 200.0f;
	m_springCamera.SetPosition(position);
	m_springCamera.Update();
}
