#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{

}
Game::~Game()
{
	DeleteGO(m_fontRender);
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();

	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetText(L"こんにちは世界");
	
	return true;
}

void Game::Update()
{
	//CPadのインスタンスを取得。
	CPad& pad = Pad(0);

	//左スティックの入力量でフォントを動かす。
	m_position.x += pad.GetLStickXF() * 3.0f;
	m_position.y += pad.GetLStickYF() * 3.0f;

	//右スティックの入力量でカラーを変える。
	//カラーは光の三原色RGBで表現されている。
	m_color.r += pad.GetRStickXF() * 0.02f;		//R成分。
	m_color.g += pad.GetRStickYF() * 0.02f;		//G成分。
	if (pad.IsPress(enButtonLeft) == true) {
		//十字キーの左が押されていたら、B成分を減らす。
		m_color.b -= 0.02f;
	}
	if (pad.IsPress(enButtonRight) == true) {
		//十字キーの右が押されていたら、R成分を減らす。
		m_color.b += 0.02f;
	}
	m_fontRender->SetPosition(m_position);
	m_fontRender->SetColor(m_color);
}

