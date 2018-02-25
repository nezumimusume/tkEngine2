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
	MainCamera().SetTarget({ 0.0f, 15.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 15.0f, 40.0f });
	MainCamera().Update();

	//モデルデータをロード。
	m_skinModelData.Load(L"modelData/Thethief_H.cmo");
	m_skinModel.Init(m_skinModelData);
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
	

	//ワールド行列を更新。
	m_skinModel.Update(CVector3::Zero, m_rotation, CVector3::One);
}
void Game::Render(CRenderContext& rc)
{
	//描画。
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}