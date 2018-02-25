#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//テクスチャをロード。
	m_texture.CreateFromDDSTextureFromFile(L"sprite/mikyan.dds");
	m_sprite.Init(m_texture, 400, 300);
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

	//ワールド行列の更新。
	m_sprite.Update(m_position, m_rotation, CVector3::One);
}

void Game::PostRender(CRenderContext& rc)
{
	//スプライトを描画。
	m_sprite.Draw(rc, MainCamera2D().GetViewMatrix(), MainCamera2D().GetProjectionMatrix());
}
