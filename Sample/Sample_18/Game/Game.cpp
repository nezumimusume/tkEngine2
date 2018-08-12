#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

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
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();

	
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
}

//フォントの描画命令はIGameObjectの仮想関数のPostRenderをオーバーライドして、
//そこで描画する。
void Game::PostRender(CRenderContext& rc)
{
	m_font.Begin(rc);	//フォントの描画開始。
	m_font.Draw(
		L"こんにちは世界",		//表示する文字列。
		m_position,				//表示する座標。0.0f, 0.0が画面の中心。
		m_color
	);

	m_font.End(rc);		//フォントの描画終了。
}