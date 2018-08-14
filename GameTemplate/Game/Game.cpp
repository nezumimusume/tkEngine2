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
	MainCamera().SetPosition({ 0.0f, 170.0f, 200.0f });
	
	MainCamera().Update();

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	//ゴーストを作成。
	/*m_ghost.CreateBox(CVector3::Zero, CQuaternion::Identity, { 100.0f, 20.0f, 100.0f });
	m_capusuleGhost.CreateCapsule({ 0.0f, 0.0f, 0.0f }, CQuaternion::Identity, 50.0f, 100.0f);
	m_sphereGhost.CreateSphere({ 0.0f, 0.0f, 0.0f }, CQuaternion::Identity, 100.0f);*/
	m_ghostPosition.x = 100.0f;
	m_meshGhost.CreateMesh(m_ghostPosition, CQuaternion::Identity, m_skinModelRender->GetSkinModelData());
	m_charaCon.Init(30.0f, 50.0f, CVector3{0.0f, 0.0f, 0.0f});
	PhysicsWorld().SetDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);
	return true;
}

void Game::Update()
{
	auto moveSpeed = CVector3::Zero;
	moveSpeed.x = -Pad(0).GetLStickXF() * 200.0f;
	moveSpeed.y = 0.0f;
//	moveSpeed.z = -Pad(0).GetLStickYF() * 200.0f;

	auto pos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), moveSpeed);
	m_skinModelRender->SetPosition(pos);
	m_timer += GameTime().GetFrameDeltaTime();
	m_ghostPosition.x = 100.0f + sinf(m_timer * 0.5f) * 100.0f;
	m_meshGhost.SetPosition(m_ghostPosition);

	m_ghostRotation.SetRotation(CVector3::AxisY, m_timer * 0.5f);
	m_meshGhost.SetRotation(m_ghostRotation);
	m_fontText = L"Not hit";
	PhysicsWorld().ContactTest(m_charaCon, [&](const auto& hitColliObject) {
		if (m_meshGhost.IsSelf(hitColliObject) == true) {
			m_fontText = L"Hit";
		}
		return 0.0f;
	});
}
void Game::PostRender(CRenderContext& rc) 
{
	m_font.Begin(rc);
	m_font.Draw(m_fontText.c_str(), { 0.0f, 300.0f });
	m_font.End(rc);
}