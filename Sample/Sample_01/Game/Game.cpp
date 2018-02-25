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
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();

	//モデルデータをロード。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	
	return true;
}
void Game::Update()
{
	//上下左右のキー入力による移動処理。
	if (Pad(0).IsPress(enButtonRight)) {
		m_position.x -= 5.0f;
	}
	else if (Pad(0).IsPress(enButtonLeft)) {
		m_position.x += 5.0f;
	}
	else if (Pad(0).IsPress(enButtonUp)) {
		m_position.y += 5.0f;
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		m_position.y -= 5.0f;
	}
	//アナログスティックによる移動処理。
	CVector3 stick;
	
	stick.x = -Pad(0).GetLStickXF();	//アナログスティックのxの入力量を取得。
	stick.y = Pad(0).GetLStickYF();	//アナログスティックのxの入力量を取得。
	stick.z = 0.0f;
	m_position += stick * 5.0f;

	//座標を設定。
	m_skinModelRender->SetPosition(m_position);
}
