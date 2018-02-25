/*!
 * @brief	プレイヤーの移動処理。
 */
#include "stdafx.h"
#include "Player/Player.h"
#include "Player/PlayerMove.h"
#include "Camera/GameCamera.h"


CPlayerMove::CPlayerMove(CPlayer* pl) :
	m_player(pl)
{
}
CPlayerMove::~CPlayerMove()
{
}
bool CPlayerMove::Start()
{
	m_charaCon.Init(15.0f, 50.0f, m_player->GetPosition());
	m_gameCamera = FindGO<CGameCamera>("GameCamera");
	//ステート切り替えのリスナーを登録。
	m_player->AddChangeStateListner([&](CPlayerConst::EnState nextState) {
		OnChangeState(nextState);
	});
	return true;
}
void CPlayerMove::OnChangeState(CPlayerConst::EnState nextState)
{
	(void)nextState;
}
void CPlayerMove::Update()
{
	//XZ平面での移動は打ち消す。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	if (m_player->IsPossibleMove() == false) {
		//移動できない。
		m_accelTime = 0.0f;
		return;
	}
	
	float lx = Pad(0).GetLStickXF();
	float ly = Pad(0).GetLStickYF();
	
	if (fabsf(lx) < 0.01f && fabsf(ly) < 0.01f) {
		m_accelTime = 0.0f;
	}
	m_accelTime = min(m_accelTime + 5.0f * GameTime().GetFrameDeltaTime(), 1.0f);

	CVector3 forward = MainCamera().GetForward();
	forward.y = 0.0f;	//Yいらない。
	forward.Normalize();
	forward.Scale(ly * CPlayerConst::RUN_SPEED * m_accelTime);
	CVector3 right = MainCamera().GetRight();
	right.y = 0.0f;		//Yいらない。
	right.Normalize();
	right.Scale(lx * CPlayerConst::RUN_SPEED * m_accelTime);

	
	//このフレームの移動速度を計算する。
	//まずはXZ平面での移動速度。
	CVector3 moveSpeed;
	moveSpeed = forward + right;
#if 1 //@todo for footIKTest
	if (m_player->IsApplyGravity()) {
		//続いてY方向。
		float addYSpeed = -980.0f * GameTime().GetFrameDeltaTime();	//重力による速度増加を計算。
		moveSpeed.y = m_moveSpeed.y + addYSpeed;
	}
#endif
	//計算終わり。
	m_moveSpeed = moveSpeed + m_addMoveSpeed;
	
#if 1 //@todo for footIKTest
	//計算された移動速度を使って移動させる。
	CVector3 pos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
#else
	CVector3 pos = m_player->GetPosition();
	pos += m_moveSpeed * GameTime().GetFrameDeltaTime();
#endif
	m_player->SetPosition(pos);
	
	//外部から加算されている力をクリアする。
	m_addMoveSpeed = CVector3::Zero;	
}