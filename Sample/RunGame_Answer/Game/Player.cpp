#include "stdafx.h"
#include "Player.h"
#include "Game.h"

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
void Player::Move()
{

}
void Player::Update()
{
	//プレイヤーの移動処理。
	Move();
	m_moveSpeed.x = Pad(0).GetLStickXF() * 200.0f;
	m_moveSpeed.y -= 20.0f;
	m_moveSpeed.z = Pad(0).GetLStickYF() * 200.0f;
	if (Pad(0).IsTrigger(enButtonA)) {
		m_moveSpeed.y = 400.0f;
	}
	m_position = m_charaCon.Execute(m_moveSpeed);
	m_skinModelRender->SetPosition(m_position);

	//スターとあたり判定を行う。
	//まず、Gameクラスのインスタンスにアクセスする。
	Game* game = FindGO<Game>("Game");
	for (int i = 0; i < 5; i++) {
		Star* star = game->m_star[i];
		if (star != nullptr) {
			//距離の判定。
			CVector3 diff = star->m_position - m_position;
			if (diff.Length() < 50.0f) {
				//距離が20以下になったら、☆を削除。
				game->m_star[i] = nullptr;
				//星の取得数をカウントアップ。
				game->m_numGetStarCount++;
				DeleteGO(star);
			}
		}
	}
}
