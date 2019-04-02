#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game() :
	m_lobbyManager( 
		L"41b72c35-96cd-4cab-ab81-cc1313e36213",
		L"tkEngine_Sample_22",
		L"1.0f")
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ƒJƒƒ‰‚ğİ’èB
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	
	return true;
}

void Game::Update()
{
	m_lobbyManager.Update();
}