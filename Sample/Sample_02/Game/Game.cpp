#include "stdafx.h"
#include "Game.h"


void Game::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
bool Game::Start()
{
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 15.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 15.0f, 40.0f });
	MainCamera().Update();

	//モデルデータをロード。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Thethief_H.cmo");
	
	return true;
}
void Game::Update()
{
	//クォータニオンを単位クォータニオンで初期化する。
	CQuaternion qRot = CQuaternion::Identity;
	//上下左右のキー入力による回転処理。
	if (Pad(0).IsPress(enButtonRight)) {
		//Y軸周りに2度回す。
		qRot.SetRotationDeg(CVector3::AxisY, 2.0f);
	}
	else if (Pad(0).IsPress(enButtonLeft)) {
		//Y軸周りに-2度回す。
		qRot.SetRotationDeg(CVector3::AxisY, -2.0f);
	}
	else if (Pad(0).IsPress(enButtonUp)) {
		qRot.SetRotationDeg(CVector3::AxisX, 2.0f);
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		qRot.SetRotationDeg(CVector3::AxisX, -2.0f);
	}
	//回転を加算する。
	m_rotation.Multiply(qRot);

	//アナログスティックによる回転処理。
	CVector3 stick;
	stick.x = -Pad(0).GetLStickXF();	//アナログスティックのxの入力量を取得。
	stick.y = Pad(0).GetLStickYF();		//アナログスティックのyの入力量を取得。
	stick.z = 0.0f;
	qRot.SetRotationDeg(CVector3::AxisY, stick.x * 2.0f);
	//回転を加算する。
	m_rotation.Multiply(qRot);

	qRot.SetRotationDeg(CVector3::AxisX, stick.y * 2.0f);
	//回転を加算する。
	m_rotation.Multiply(qRot);
	
	//回転を設定。
	m_skinModelRender->SetRotation(m_rotation);
}
