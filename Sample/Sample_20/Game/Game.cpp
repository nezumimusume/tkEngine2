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
	MainCamera().SetFar(10000.0f);
	MainCamera().Update();

	//スキンモデルレンダラーを作成。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	return true;
}

void Game::Update()
{
	return;
	if (m_rotateMode == 0) {
		//自動回転モード。
		//まず、Y軸周りに1度回転するクォータニオンを計算する。
		CQuaternion qAddRot;
		qAddRot.SetRotationDeg(CVector3::AxisY, 1.0f);
		//m_rotationにqAddRotを乗算して代入
		//クォータニオンは乗算すると回転が合成される。
		m_rotation *= qAddRot;
		//合成したクォータニオンをスキンモデルレンダラーに伝える。
		m_skinModelRender->SetRotation(m_rotation);
	}
	else if (m_rotateMode == 1) {
		//スティックで回転させるモード。
		CQuaternion qAddRotY, qAddRotX;
		qAddRotY.SetRotationDeg( CVector3::AxisY, Pad(0).GetLStickXF() );
		qAddRotX.SetRotationDeg( CVector3::AxisX, Pad(0).GetLStickYF() );
		//回転を加算する。
		m_rotation *= qAddRotY;
		m_rotation *= qAddRotX;
		//合成したクォータニオンをスキンモデルレンダラーに伝える。
		m_skinModelRender->SetRotation(m_rotation);
	}
	if (Pad(0).IsTrigger(enButtonA) == true) {
		//Aボタンを押したら、回転モードを変更する。
		if (m_rotateMode == 0) {
			m_rotateMode = 1;
		}else if (m_rotateMode == 1) {
			m_rotateMode = 0;
		}
	}
}
