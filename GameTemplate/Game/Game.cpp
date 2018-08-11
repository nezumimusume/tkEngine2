#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
	DeleteGO(m_skinModelRender);
}

class Hoge {
};
Game::~Game()
{
}
bool Game::Start()
{
	//ƒJƒƒ‰‚ğİ’èB
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, 5000.0f });
	MainCamera().Update();

	m_animClips[0].Load(L"animData/test.tka");
	m_animClips[0].SetLoopFlag(true);
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo", m_animClips, 1);
	auto bone = m_skinModelRender->FindBone(L"Bone018");
	auto localMatrix = bone->GetLocalMatrix();

	SetOutputDebugVectorEnable(true);
	return true;
}

void Game::Update()
{
	if (Pad(0).IsPress(enButtonA) ){
		pos.z += 10.0f;
	}
}