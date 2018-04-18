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
	m_animClip[0].Load(L"animData/test.tka");
	m_animClip[0].SetLoopFlag(true);
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo", m_animClip, 1);
	m_skinModelRender->SetScale({ 0.1f, 0.1f, 0.1f } );
	
	//エネミーを２体生成する。
	NewGO<Enemy>(0, "Enemy01");
	NewGO<Enemy>(0, "Enemy02");
	m_player = NewGO<Player>(0);
	//Enemy01という名前が付けられている、インスタンスを検索する。
	Enemy* enemy = FindGO<Enemy>("Enemy01");
	return true;
}
void Game::Update()
{
}
void Game::Render(CRenderContext& rc)
{
}