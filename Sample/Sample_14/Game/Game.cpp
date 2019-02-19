#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "Player.h"
#include "GameCamera.h"
#include "Star.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
	m_lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig = NewGO<prefab::CDirectionLight>(0);
	//ライトの方向を設定。
	m_lig->SetDirection(m_lightDir);
	//ライトの色を設定。
	m_lig->SetColor({ 300.5f, 300.5f, 300.5f, 1.0f });

	dbg::SetDrawPhysicsCollisionEnable();
	//GameCameraのインスタンスを生成する。
	m_gameCamera = NewGO<GameCamera>(0);
	//レベルを構築する。
	m_level.Init(L"level/stage_00.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"star") == true) {
			
		}
		else if (objData.EqualObjectName(L"UnityChan") == true) {
			//Unityちゃん。
			//プレイヤーのインスタンスを生成する。
			m_player = NewGO<Player>(0, "Player");
			m_player->m_position = objData.position;
			//フックした場合はtrueを返す。
			return true;
		}
		return false;
	});
	return true;
}
/*!
*@brief	削除されるときに呼ばれる。
*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
* デストラクタより前に実行されます。
*/
void Game::OnDestroy()
{
	DeleteGO("Player");
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	for (auto& star : m_starList) {
		DeleteGO(star);
	}
}
void Game::Update()
{
	if (Pad(0).IsPress(enButtonA)) {
		DeleteGO(this);
		NewGO<Game>(0);
	}
}
