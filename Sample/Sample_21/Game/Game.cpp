#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"

Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_directionLight);
	DeleteGO(m_unityChanModelRender);
	DeleteGO(m_bgModelRender);
	DeleteGO(m_starModelRender);
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 200.0f, 500.0f });
	MainCamera().Update();

	//ライトを一本追加。
	m_directionLight = NewGO<prefab::CDirectionLight>(0);
	m_directionLight->SetDirection({ 0.707f, -0.707f, 0.0f });
	m_directionLight->SetColor({2.0f, 2.0f, 2.0f, 1.0f});

	//ユニティちゃんのスキンモデルレンダラーを作成。
	m_unityChanModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_unityChanModelRender->Init(L"modelData/unityChan.cmo");
	m_unityChanModelRender->SetShadowCasterFlag(true);
	//背景モデルのレンダラーを作成。
	m_bgModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_bgModelRender->Init(L"modelData/bg/bg.cmo");
	m_bgModelRender->SetShadowReceiverFlag(true);
	//星のモデルのレンダラーを作成する。
	m_starModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_starModelRender->Init(L"modelData/star/star.cmo");
	m_starModelRender->SetScale({ 20.0f, 20.0f, 20.0f });
	CVector3 starPos = { 0.0f, 200.0f, 0.0f };
	m_starModelRender->SetPosition(starPos);

	//全方位ライトを設定する。
	//ライトの座標を設定する。
	shadow::OminiDirectionShadowMap().SetLightPosition(starPos);
	//全方位シャドウの影響範囲を設定する。
	shadow::OminiDirectionShadowMap().SetDistanceAffectedByLight(1000);

	//全方位シャドウの効果を分かりやすくするために、指向性シャドウは無効にしておく。
	shadow::DirectionShadowMap().Disable();
	return true;
}
void Game::Update()
{
	//ゲームパッドの入力でUnityちゃんを動かす。
	m_unityChanPosition.x -= Pad(0).GetLStickXF() * 5.0f;
	m_unityChanPosition.z -= Pad(0).GetLStickYF() * 5.0f;
	m_unityChanModelRender->SetPosition(m_unityChanPosition);
}

