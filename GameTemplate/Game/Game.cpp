#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ÉJÉÅÉâÇê›íËÅB
	MainCamera().SetTarget({ 0.0f, 0.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);
	MainCamera().SetPosition({ 0.0f, 0.0f, 200.0f });
	MainCamera().Update();
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/star.cmo");
	m_skinModelRender->SetScale({ 1.0f, 1.0f, 1.0f });
	m_skinModelRender->SetEmissionColor({ 5.0f, 5.0f, 0.0f });
	LightManager().SetAmbientLight({ 2.5f, 2.5f, 2.5f });
	auto& dof = GraphicsEngine().GetPostEffect().GetDof();
	dof.Enable();
	dof.SetDofRangeParam(0, 150, 160, 400);
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