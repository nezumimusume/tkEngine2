#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "Background.h"
#include "Ball.h"

namespace {
	const char* cDIR_LIGHT_NAME = "dirLight";
	const char* cBG_NAME = "背景";
	const char* cBALL_NAME = "ボール";
}
Game::Game()
{
}


Game::~Game()
{
	//ゲームオブジェクトを名前指定で削除。
	DeleteGOs(cDIR_LIGHT_NAME);
	DeleteGOs(cBG_NAME);
}
void Game::InitLight()
{
	auto dirLight = NewGO<prefab::CDirectionLight>(0, cDIR_LIGHT_NAME);
	dirLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	auto lightDir = CVector3(1, -1, 1);
	lightDir.Normalize();
	dirLight->SetDirection(lightDir);
	GraphicsEngine().GetShadowMap().SetLightDirection(lightDir);

	dirLight = NewGO<prefab::CDirectionLight>(0, cDIR_LIGHT_NAME);
	dirLight->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	dirLight->SetDirection({ 0.0f, -0.707f, 0.707f });

	dirLight = NewGO<prefab::CDirectionLight>(0, cDIR_LIGHT_NAME);
	dirLight->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	dirLight->SetDirection({ 0.0f, -0.707f, -0.707f });

	dirLight = NewGO<prefab::CDirectionLight>(0, cDIR_LIGHT_NAME);
	dirLight->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	dirLight->SetDirection({ 0.707f, -0.707f, 0.0f });
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 0.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);
	MainCamera().SetPosition({ 0.0f, 300.0f, 300.0f });
	MainCamera().Update();
	
	dbg::SetDrawPhysicsCollisionEnable();
	//物理エンジンの重力を設定する。
	PhysicsWorld().SetGravity({ 0.0f, -980.0f, 0.0f });
	//ライトを初期化。	
	InitLight();
	//背景を作成。
	NewGO<Background>(0, cBG_NAME);
	NewGO<Ball>(0, cBALL_NAME);
	return true;
}

void Game::Update()
{
}