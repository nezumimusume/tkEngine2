#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

CVector3 cameraPos = { 0.0f, 70.0f, 200.0f };
CVector3 cameraTarget;
Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ÉJÉÅÉâÇê›íËÅB
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
	float speed = 1.0f;
	if (Pad(0).IsPress(enButtonB)) {
		speed *= 10.0f;
	}
	if (Pad(0).IsPress(enButtonUp)) {
		cameraPos.z -= speed;
	}
	if (Pad(0).IsPress(enButtonDown)) {
		cameraPos.z += speed;
	}

	MainCamera().SetPosition(cameraPos);
	MainCamera().Update();
}
