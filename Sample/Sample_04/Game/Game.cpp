#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"


void Game::OnDestroy()
{
	DeleteGO(m_lig);
	DeleteGO(m_skinModelRender);
}
bool Game::Start()
{
	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
	m_lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig = NewGO<prefab::CDirectionLight>(0);
	//ライトの方向を設定。
	m_lig->SetDirection(m_lightDir);
	//ライトの色を設定。
	m_lig->SetColor({ 300.5f, 300.5f, 300.5f, 1.0f });


	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 15.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 15.0f, 30.0f });
	MainCamera().Update();

	//スキンモデルレンダラーを設定。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Thethief_H.cmo");
	

	return true;
}
void Game::Update()
{
	//ライトを回す。
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 2.0f);
	qRot.Multiply(m_lightDir);
	m_lig->SetDirection(m_lightDir);
}
