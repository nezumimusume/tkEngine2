#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	MainCamera().SetPosition({ 0.0f, 170.0f, -500.0f});
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().Update();
	return true;
}
void GameCamera::Update()
{
	//プレイヤーの座標から、注視点と視点を計算する。
	//まずはプレイヤーのインスタンスを検索する。
	Player* player = FindGO<Player>("プレイヤー");
	//注視点を計算する。
	CVector3 target = player->m_position;
	target.y += 70.0f;	//注視点はプレイヤーのちょっと上にする。
	
	//続いて視点を計算する。
	CVector3 position = target;
	//視点は注視点からy方向に+100、Z方向に-500にする。
	position.y += 100.0f;
	position.z -= 500.0f;
	//メインカメラに注視点と視点を設定して更新する。
	MainCamera().SetTarget(target);
	MainCamera().SetPosition(position);
	MainCamera().Update();
}