#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "ground.h"
#include "Player.h"
#include "GameCamera.h"
#include "GameObjectName.h"
#include "StarGenerator.h"
#include "ScoreHUD.h"
#include "TimerHUD.h"
#include "Title.h"

namespace {	
	const char* GROUND_NAME = "地面";
	const char* GAME_CAMERA_NAME = "カメラ";
	const char* DIRECTION_LIG_NAME = "ディレクションライト";
	const char* STAR_GENERATOR_NAME = "星生成器";
	const char* SCORE_HUD = "スコアのHUD";
	const char* TIMER_HUD_NAME = "タイマーのHUD";
}
Game::Game()
{
}


Game::~Game()
{
	DeleteGO( GROUND_NAME );
	DeleteGO( PLAYER_NAME );
	DeleteGO( GAME_CAMERA_NAME );
	DeleteGOs(DIRECTION_LIG_NAME);
	DeleteGOs(STAR_NAME);
	DeleteGO(STAR_GENERATOR_NAME);
	DeleteGO(SCORE_HUD);
	DeleteGO(m_bgm);
	DeleteGO(TIMER_HUD_NAME);
}
bool Game::Start()
{
	auto dirLig = NewGO<prefab::CDirectionLight>(0, "ディレクションライト");
	dirLig->SetDirection({ 0.0f, -1.0f, 0.0f });
	dirLig->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });
	GraphicsEngine().GetShadowMap().SetLightDirection(dirLig->GetDirection());
	LightManager().SetAmbientLight({ 0.1f, 0.1f, 0.1f });

	NewGO< Player>(0, PLAYER_NAME);
	NewGO< GameCamera>(0, GAME_CAMERA_NAME);
	NewGO<StarGenerator>(0, STAR_GENERATOR_NAME);
	NewGO< ScoreHUD>(0, SCORE_HUD);
	NewGO<TimerHUD>(0, TIMER_HUD_NAME);
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->Init(L"sound/bgm.wav");
	m_bgm->Play(false);
	m_level.Init(L"level/level_00.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"bg/bg") == true) {
			auto ground = NewGO<Ground>(0, GROUND_NAME);
			return true;
		}
		return false;
	});
	
	return true;
}

void Game::Update()
{
	m_restTimer = max(0.0f, m_restTimer - GameTime().GetFrameDeltaTime());
	switch (m_state) {
	case enState_Playing:
		if (m_restTimer == 0.0f) {
			//終わり。
			m_state = enState_Timeup;
		}
		break;
	case enState_Timeup:
		m_clearEffectTimer += GameTime().GetFrameDeltaTime();
		if (m_clearEffectTimer > 5.0f) {
			//5秒経過。
			DeleteGO(this);
			NewGO<Title>(0);
		}
		break;
	}
	
}