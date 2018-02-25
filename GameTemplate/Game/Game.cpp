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
	MainCamera().SetTarget({ 0.0f, 10.0f, 0.0f });
	MainCamera().SetNear(0.1f);
	MainCamera().SetFar(100.0f);
	MainCamera().SetPosition({ 30.0f, 10.0f, 0.0f });
	MainCamera().Update();
	
	//モデルデータをロード。
	skinModelData.Load(L"modelData/unityChan.cmo");
	skinModel.Init(skinModelData);
	
	return true;
}
void Game::Update()
{
	skinModel.Update(m_pos, CQuaternion::Identity, { 0.1f, 0.1f, 0.1f });
}
void Game::Render(CRenderContext& rc)
{
	skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}