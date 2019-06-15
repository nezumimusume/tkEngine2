#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_spriteRender);
}
bool Game::Start()
{
	//スプライトを初期化。
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/mikyan.dds", 400, 300);

	return true;
}
void Game::Update()
{
	//回転
	CQuaternion qAdd;
	qAdd.SetRotationDeg(CVector3::AxisZ, 1.0f);
	m_rotation.Multiply(m_rotation, qAdd);
	//移動。スティックの入力で移動させる。
	m_position.x += Pad(0).GetLStickXF() * 10.0f;
	m_position.y += Pad(0).GetLStickYF() * 10.0f;
	m_position.z = 0.0f;	//2D空間で描画するならzは0でいい。

	m_scale.x += Pad(0).GetRStickXF() * 0.1f;
	m_scale.y += Pad(0).GetRStickYF() * 0.1f;

	//座標と回転を設定。
	m_spriteRender->SetPosition(m_position);
	m_spriteRender->SetRotation(m_rotation);
	m_spriteRender->SetScale(m_scale);
}

