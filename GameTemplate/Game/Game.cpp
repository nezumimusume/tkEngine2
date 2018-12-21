#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_uniSkinModelRender);
	DeleteGO(m_uniSkinModelRender2);
	DeleteGO(m_bgSkinModelRender);
	DeleteGO(m_sky);
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
	m_uniSkinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_uniSkinModelRender->Init(L"modelData/unityChan.cmo");
	m_uniSkinModelRender->SetPosition({ 30.0f, 0.0f, 150.0f });
	m_uniSkinModelRender2 = NewGO<prefab::CSkinModelRender>(0);
	m_uniSkinModelRender2->Init(L"modelData/unityChan.cmo");
	m_uniSkinModelRender2->SetPosition({ -30.0f, 0.0f, -150.0f });

	m_bgSkinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_bgSkinModelRender->Init(L"modelData/bg/bg.cmo");
	m_sky = NewGO<prefab::CSky>(0);
	LightManager().SetAmbientLight({ 2.5f, 2.5f, 2.5f });
	return true;
}

void Game::Update()
{
	CVector3 toPos = MainCamera().GetPosition() - MainCamera().GetTarget();
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY, Pad(0).GetLStickXF() * 0.05f);
	rot.Apply(toPos);
	auto pos = MainCamera().GetTarget();
	pos += toPos;
	MainCamera().SetPosition(pos);
	MainCamera().Update();
}