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
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 70.0f, -200.0f });
	MainCamera().Update();

	//モデルデータをロード。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	
	return true;
}
void Game::Update()
{

	//アナログスティックによる移動処理。
	//GetLStickXFは、ゲームパッドの横方向の倒れ具合で-1.0～1.0を返してきます。
	//GetLStickYFは、ゲームパッドの縦方向の倒れ具合で-1.0～1.0を返してきます。
	m_position.x += Pad(0).GetLStickXF() * 5.0f;
	m_position.z += Pad(0).GetLStickYF() * 5.0f;

	//座標を設定。
	m_skinModelRender->SetPosition(m_position);
}
