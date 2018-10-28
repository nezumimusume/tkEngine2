#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "ground.h"
#include "Player.h"
#include "GameCamera.h"
#include "GameObjectName.h"
#include "StarGenerator.h"

namespace {	
	const char* GROUND_NAME = "地面";
	const char* GAME_CAMERA_NAME = "カメラ";
	const char* DIRECTION_LIG_NAME = "ディレクションライト";
	const char* STAR_GENERATOR_NAME = "星生成器";
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
	DeleteGO(m_fontRender);
}
bool Game::Start()
{
	
	MainCamera().Update();

	auto dirLig = NewGO<prefab::CDirectionLight>(0, "ディレクションライト");
	dirLig->SetDirection({ 0.0f, -1.0f, 0.0f });
	dirLig->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });
	GraphicsEngine().GetShadowMap().SetLightDirection(dirLig->GetDirection());
	LightManager().SetAmbientLight({ 0.1f, 0.1f, 0.1f });

	NewGO< Player>(0, PLAYER_NAME);
	NewGO< GameCamera>(0, GAME_CAMERA_NAME);
	NewGO<StarGenerator>(0, STAR_GENERATOR_NAME);
	m_fontRender = NewGO<prefab::CFontRender>(0);
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
	m_fontRender->SetText(L"ほげぇぇぇぇぇ");
}