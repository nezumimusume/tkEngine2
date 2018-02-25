#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"
#include "BackGround/BackGround.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "Enemy\Enemy.h"
#include "tkEngine/light/tkPointLight.h"
#include "tkEngine/util/tkLocData.h"

CVector3 toPosition = {0.0f, 30.0f, 100.0f};
Game::Game()
{
}


Game::~Game()
{
}
void Game::OnDestroy()
{
	for (auto enemy : m_enemyList) {
		DeleteGO(enemy);
	}
	for (auto pointLight : m_pointLightList) {
		DeleteGO(pointLight);
	}
	DeleteGO(&m_player);
	DeleteGO(&m_bg);
	DeleteGO(m_directionLig);
	DeleteGO(&m_gameCamera);
}
bool Game::Start()
{
	m_directionLig = NewGO<prefab::CDirectionLight>(0, nullptr);
	CVector3 lightDir = { 0.707f, -0.707f, -0.707f };
	lightDir.Normalize();
	m_directionLig->SetDirection(lightDir);
	m_directionLig->SetColor({ 1.4f, 1.4f, 1.4f, 1.0f });
	GraphicsEngine().GetShadowMap().SetLightDirection(lightDir);
	LightManager().SetAmbientLight({ 0.2f, 0.2f, 0.2f });
	AddGO(0, &m_player, "Player");
	
	CLocData enemyLoc;
	enemyLoc.Load(L"loc/enemyLoc.tks");
	for (int i = 0; i < enemyLoc.GetNumObject(); i++) {
		auto enemy = NewGO<Enemy>(0, "Enemey");
		m_enemyList.push_back(enemy);
		CVector3 pos = enemyLoc.GetObjectPosition(i);
		pos.y += 10.0f;
		enemy->SetPosition(pos);
	}
	//ポイントライトを配置。
	CLocData ligLoc;
	ligLoc.Load(L"loc/pointLightLoc.tks");
	for (int i = 0; i < ligLoc.GetNumObject(); i++) {
		auto pointLig = NewGO<prefab::CPointLight>(0);
		pointLig->SetPosition(ligLoc.GetObjectPosition(i));
		pointLig->SetColor({
			10.0f,
			10.0f,
			8.0f,
			1.0f
		});
		pointLig->SetAttn({
			UnitM(1.5f),
			3.0f,
			0.1f
		});
		m_pointLightList.push_back(pointLig);
	}
	AddGO(0, &m_bg, "BackGround");
	AddGO(0, &m_gameCamera, "GameCamera");
	
	return true;
}
void Game::Update()
{
}
void Game::Render(CRenderContext& rc)
{
	(void)rc;
}