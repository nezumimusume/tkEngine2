#include "stdafx.h"
#include "GameOverControl.h"
#include "GameOverCamera.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"
#include "GameCamera.h"

GameOverControl::GameOverControl()
{
	//
	FindGO<Player>("Player")->NotifyGameOver();
	FindGO<GameCamera>("GameCamera")->NotifyGameOver();
}
GameOverControl::~GameOverControl()
{
}
void GameOverControl::OnDestroy()
{
	DeleteGO(m_gameOverCamera);
}
bool GameOverControl::Start()
{
	//ゲームオーバーカメラを作成。
	Game* game = FindGO<Game>("Game");
	//プレイヤーを発見したエネミーを検索する。
	Enemy* enemy = game->FindEnemy([&](Enemy* enemy) {
		return enemy->IsFindPlayer(); 
	});
	if (enemy) {
		m_gameOverCamera = NewGO<GameOverCamera>(0, "GameOverCamera");
		m_gameOverCamera->Init(*FindGO<Player>("Player"), *enemy);
	}
	return true;
}
void GameOverControl::Update()
{
}
