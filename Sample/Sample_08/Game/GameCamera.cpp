#include "stdafx.h"
#include "GameCamera.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");
	//カメラのニアクリップとファークリップを設定する。
	MainCamera().SetNear(1.0f);
	MainCamera().SetFar(10000.0f);
	return true;
}
void GameCamera::Update()
{
	//カメラを更新。
	//注視点を計算する。
	CVector3 target = m_player->m_position;
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;
	//視点を計算する。
	CVector3 toPos = { 0.0f, 50.0f, 150.0f };
	CVector3 pos = target + toPos;	//キャラクタを斜め上から見ているような視点にする。
	//メインカメラに注視点と視点を設定する。
	MainCamera().SetTarget(target);
	MainCamera().SetPosition(pos);

	//カメラの更新。
	MainCamera().Update();
}