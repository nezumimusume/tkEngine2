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
	MainCamera().SetTarget({ 0.0f, 100.0f, 0.0f });
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1000.0f);
	MainCamera().SetPosition({ 0.0f, 100.0f, 200.0f });
	MainCamera().Update();

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo");
	m_bgSkinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_bgSkinModelRender->Init(L"modelData/background.cmo");
	m_spec.CreateFromDDSTextureFromFile(L"sprite/test.dds");
	m_bgSkinModelRender->FindMaterial([&](CModelEffect* mat) {
	//	if (mat->EqualMaterialName(L"Ground") == true){
			mat->SetSpecularMap(m_spec.GetBody());
	//	}
	});
	return true;
}

void Game::Update()
{
	//左スティックの入力量を受け取る。
	float lStick_x = Pad(0).GetLStickXF();
	float lStick_y = Pad(0).GetLStickYF();
	//カメラの前方方向と右方向を取得。
	CVector3 cameraForward = MainCamera().GetForward();
	CVector3 cameraRight = MainCamera().GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	CVector3 toPos = MainCamera().GetPosition() - MainCamera().GetTarget();
	pos += cameraForward * lStick_y * 2.0f;	//奥方向への移動速度を加算。
	pos += cameraRight * lStick_x * 2.0f;		//右方向への移動速度を加算。

	m_skinModelRender->SetPosition(pos);
	//カメラを更新。
	//注視点を計算する。
	CVector3 target = pos;
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;

	CVector3 toCameraPosOld = toPos;
	//パッドの入力を使ってカメラを回す。
	float x = Pad(0).GetRStickXF();
	float y = Pad(0).GetRStickYF();
	//Y軸周りの回転
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 2.0f * x);
	qRot.Multiply(toPos);
	//X軸周りの回転。
	CVector3 axisX;
	axisX.Cross(CVector3::AxisY, toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(toPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	CVector3 toPosDir = toPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//カメラが上向きすぎ。
		toPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//カメラが下向きすぎ。
		toPos = toCameraPosOld;
	}

	//視点を計算する。
	CVector3 pos = target + toPos;
	//メインカメラに注視点と視点を設定する。
	MainCamera().SetTarget(target);
	MainCamera().SetPosition(pos);

	//カメラの更新。
	MainCamera().Update();
}