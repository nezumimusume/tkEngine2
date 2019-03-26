#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "tkEngine/light/tkPointLight.h"

Game::Game()
{

}
Game::~Game()
{
	DeleteGO(m_backGroundRender);
	DeleteGO(m_playerRender);
	DeleteGO(m_pointLig);
}
bool Game::Start()
{
	//カメラを初期化。
	InitCamera();
	//スキンモデルレンダラーを初期化。
	InitSkinModelRender();
	//ゴーストの初期化。
	InitGhostObject();

	m_charaCon.Init(30.0f, 100.0f, CVector3::Zero);
	//ポイントライトを作成。
	m_pointLig = NewGO<prefab::CPointLight>(0);
	m_pointLig->SetPosition({ -727.52f, 500.0f, 29.45f });
	m_pointLig->SetColor({ 100.0f, 0.0f, 0.0f });
	m_pointLig->SetAttn({ 800.f, 10.0f, 0.0f });
	m_pointLig->SetActiveFlag(false); //非アクティブにしておく。
	return true;
}
void Game::InitSkinModelRender()
{
	m_backGroundRender = NewGO<prefab::CSkinModelRender>(0);
	m_backGroundRender->Init(L"modelData/background/background.cmo");

	m_playerRender = NewGO<prefab::CSkinModelRender>(0);
	m_playerRender->Init(L"modelData/player/unityChan.cmo");
}
void Game::InitCamera()
{
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);
	MainCamera().SetPosition({ 0.0f, 570.0f, 1000.0f });
	MainCamera().Update();
}
void Game::InitGhostObject()
{
	//ゴーストのワイヤーフレーム表示を有効にする。
	PhysicsWorld().SetDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);
	//ボックス形状のゴーストを作成する。
	m_ghostObject.CreateBox(
		{ -500.0f, 100.0f, 0.0f },	//第一引数は座標。
		CQuaternion::Identity,		//第二引数は回転クォータニオン。
		{200.0f, 200.0f, 200.0f}	//第三引数はボックスのサイズ。
	);
}

void Game::Update()
{
	//パッドの入力でキャラを動かす。
	CVector3 moveSpeed;
	moveSpeed.x = Pad(0).GetLStickXF() * -300.0f;
	moveSpeed.y = 0.0f;
	moveSpeed.z = Pad(0).GetLStickYF() * -300.0f;
	//moveSpeedに設定された速度で、キャラクターを動かす。
	m_charaCon.Execute(moveSpeed);
	//キャラコンで動かした結果をCSkinModelRenderに反映させる。
	m_playerRender->SetPosition(m_charaCon.GetPosition());

	m_pointLig->SetActiveFlag(false); //非アクティブにしておく。
	//キャラクターとゴーストのあたり判定を行う。
	PhysicsWorld().ContactTest(m_charaCon,	[&](const btCollisionObject& contactObject) {
		if (m_ghostObject.IsSelf(contactObject) == true) {
			//m_ghostObjectとぶつかった
			m_pointLig->SetActiveFlag(true);	//ポイントライトをつける。
		}
	});
}