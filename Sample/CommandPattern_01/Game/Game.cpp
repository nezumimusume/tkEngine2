#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "GameActor.h"
#include "Background.h"
#include "GameCamera.h"
#include "Player.h"
Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//アクターのインスタンスを生成する。
	m_actor = NewGO<GameActor>(0, "暗殺者");	
	//Playerを作成。
	m_player = NewGO<Player>(0);
	
	m_lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig = NewGO<prefab::CDirectionLight>(0);
	m_lig->SetDirection(m_lightDir);
	m_lig->SetColor({ 0.5f, 0.5f, 0.5f, 1.0f });
	LightManager().SetAmbientLight({ 0.1f, 0.1f, 0.1f });
	m_background = NewGO<Background>(0);
	m_gameCamera = NewGO<GameCamera>(0);
	return true;
}
/*!
*@brief	削除されるときに呼ばれる。
*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
* デストラクタより前に実行されます。
*/
void Game::OnDestroy()
{
	DeleteGO(m_actor);
	DeleteGO(m_background);
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
}
void Game::Update()
{
	if (GetAsyncKeyState(VK_SPACE)) {
		//記録を再生する。
		m_actor->PlayRecord();
	}
}
