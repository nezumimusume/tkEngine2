#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 70.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 70.0f, 200.0f });
	MainCamera().Update();

	//スキンモデルレンダラーを作成。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	return true;
}

void Game::Update()
{
	//まず、Y軸周りに1度回転するクォータニオンを計算する。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY, 1.0f);
	//m_rotationにqAddRotを乗算して代入
	//クォータニオンは乗算すると回転が合成される。
	m_rotation *= qAddRot;
	//合成したクォータニオンをスキンモデルレンダラーに伝える。
	m_skinModelRender->SetRotation(m_rotation);
}
