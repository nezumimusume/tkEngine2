#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
	DeleteGO(m_skinModelRender);
}


Game::~Game()
{
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1000.0f);
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	//ゴーストを作成。
	m_ghost.Create(CVector3::Zero, CQuaternion::Identity, { 20.0f, 20.0f, 20.0f });
	m_charaCon.Init(10.0f, 20.0f, CVector3{0.0f, 0.0f, 0.0f});
	return true;
}

void Game::Update()
{
	auto moveSpeed = CVector3::Zero;
	moveSpeed.x = -Pad(0).GetLStickXF() * 200.0f;
	moveSpeed.y = 0.0f;
	moveSpeed.z = -Pad(0).GetLStickYF() * 200.0f;

	auto pos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), moveSpeed);
	m_skinModelRender->SetPosition(pos);

	PhysicsWorld().ContactTest(m_charaCon, [&](const auto& hitColliObject) {
		if (m_ghost.IsSelf(hitColliObject) == true) 
		{
			m_ghost.Release();
			MessageBox(nullptr, "hoge", "hoge", MB_OK);
		}
		return 0.0f;
	});
}