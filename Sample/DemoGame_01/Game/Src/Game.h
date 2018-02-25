#pragma once

#include "Camera/GameCamera.h"
#include "Player/Player.h"
#include "BackGround/BackGround.h"
#include "tkEngine/light/tkPointLight.h"

class Enemy;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	void OnDestroy() override;
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
private:
	CPlayer m_player;			//!<プレイヤー
	CBackGround m_bg;			//!<背景。
	CGameCamera m_gameCamera;	//!<ゲームカメラ。
	std::vector<Enemy*> m_enemyList;	//!<敵のリスト。
	std::vector<prefab::CPointLight*>	m_pointLightList;	//!<ポイントライトのリスト。
	prefab::CDirectionLight* m_directionLig = nullptr;	//!<ディレクションライト。
};

