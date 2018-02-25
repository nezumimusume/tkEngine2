/*!
 * @brief	ゲームカメラ。
 */

#include "stdafx.h"
#include "Camera/GameCamera.h"
#include "Player/Player.h"

namespace {
	float TARGET_OFFSET_Y = 60.0f;		//アクターから注視点のYオフセット。
}
bool CGameCamera::Start()
{
	m_player = FindGO<CPlayer>("Player");
	m_springCamera.Init(MainCamera(), 500.0f, true, 5.0f);
	CVector3 toCameraPos = { 0.0f, 0.0f, 115.0f };

	CVector3 target = m_player->GetPosition();
	target.y += TARGET_OFFSET_Y;

	CVector3 pos;
	pos.Add(target, toCameraPos);

	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
	m_springCamera.Update();

	m_toCameraPosDist = toCameraPos.Length();

	m_springCamera.SetFar(10000.0f);
	m_springCamera.SetNear(1.0f);
	m_springCamera.SetDampingRate(1.0f);


	return true;
}

void CGameCamera::Update()
{
	//カメラをRスティックを使って回転させる。
	float rStick_x = Pad(0).GetRStickXF();
	float rStick_y = Pad(0).GetRStickYF();
	CVector3 toCameraPos = m_springCamera.GetPosition() - m_springCamera.GetTarget();
	const float CAMERA_ROT_SPEED = 2.0f;
	if (fabsf(rStick_x) > 0.0f) {
		//Y軸周りの回転を計算。
		//回す
		CMatrix mRot;
		mRot.MakeRotationY(CAMERA_ROT_SPEED * rStick_x * GameTime().GetFrameDeltaTime());
		mRot.Mul(toCameraPos);
	}
	if (fabsf(rStick_y) > 0.0f) {
		CVector3 rotAxis;
		rotAxis.Cross(CVector3::Up, toCameraPos);
		rotAxis.Normalize();
		CMatrix mRot;
		mRot.MakeRotationAxis(rotAxis, CAMERA_ROT_SPEED * rStick_y * GameTime().GetFrameDeltaTime());
		CVector3 toPositionOld = toCameraPos;
		mRot.Mul(toCameraPos);
		CVector3 toPosDir = toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//カメラが上向きすぎ。
			toCameraPos = toPositionOld;
		}
		else if (toPosDir.y > 0.8f) {
			//カメラが下向きすぎ。
			toCameraPos = toPositionOld;
		}
	}

	//新しい注視点をプレイヤーの座標から決める。
	CVector3 target = m_player->GetPosition();
	target.y += TARGET_OFFSET_Y;

	if (fabsf(rStick_x) < 0.01f) {
		//Rスティックの入力がなければオートカメラの処理を行う。
		//現在のカメラでのXZ平面上での注視点から視点までのベクトルと、その長さを求める。
		CVector3 toCameraPosXZ = toCameraPos;
		float height = toCameraPosXZ.y;						//視点へのY方向の高さは記憶しておく。
		toCameraPosXZ.y = 0.0f;								//XZ平面にするので、Yは0にする。
		float toCameraPosXZLen = toCameraPosXZ.Length();	//XZ平面上での視点と注視点の距離を求める。
		toCameraPosXZ.Normalize();

		//XZ平面での新しいカメラの視点を決める。
		//オートカメラ
		toCameraPos = m_springCamera.GetPosition() - target;
		toCameraPos.y = 0.0f;
		toCameraPos.Normalize();
		//ちょっとづつ追尾。
		float weight = 0.5f;	//このウェイトの値は0.0～1.0の値をとる。1.0に近づくほど追尾が強くなる。
		toCameraPos = toCameraPos * weight + toCameraPosXZ * (1.0f - weight);
		toCameraPos.Normalize();
		toCameraPos *= toCameraPosXZLen;
		toCameraPos.y = height;					//高さを戻す。
	}

	CVector3 pos = target + toCameraPos;	//これで新しい視点が確定。

	//視点と注視点を設定。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);


	m_springCamera.Update();
	
}