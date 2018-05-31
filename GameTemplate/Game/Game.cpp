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
	//ƒJƒƒ‰‚ğİ’èB
	MainCamera().SetTarget({ 0.0f, 10.0f, 0.0f });
	MainCamera().SetNear(0.1f);
	MainCamera().SetFar(100.0f);
	MainCamera().SetPosition({ 30.0f, 10.0f, 0.0f });
	MainCamera().Update();

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	m_skinModelRender->SetFrustumCulling(true);
	m_skinModelRender->SetScale({ 0.1f, 0.1f, 0.1f } );
	
	return true;
}
void Game::Update()
{
	auto cameraPos = MainCamera().GetPosition();
	auto cameraTarget = MainCamera().GetTarget();
	if (Pad(0).IsPress(enButtonLeft)) {
		cameraPos.z += 5.0f;
		cameraTarget.z += 5.0f;
	}
	if (Pad(0).IsPress(enButtonRight)) {
		cameraPos.z -= 5.0f;
		cameraTarget.z -= 5.0f;
	}
	MainCamera().SetPosition(cameraPos);
	MainCamera().SetTarget(cameraTarget);
	MainCamera().Update();
}
void Game::Render(CRenderContext& rc)
{
}