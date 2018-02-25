/*!
 * @brief	プレイヤー
 */

#include "stdafx.h"
#include "Player/Player.h"
#include "Player/PlayerRenderer.h"
#include "Player/Animation/PlayerAnimator.h"
#include "tkEngine/graphics/effect/tkEffect.h"


CPlayer::CPlayer() :
	m_playerRenderer(this),
	m_playerAnimator(this),
	m_playerMove(this),
	m_playerTurn(this),
	m_playerStateMachine(this)
{
}
CPlayer::~CPlayer()
{
}

void CPlayer::OnDestroy()
{
	DeleteGO(&m_playerRenderer);
	DeleteGO(&m_playerAnimator);
	DeleteGO(&m_playerStateMachine);
	DeleteGO(&m_playerMove);
	DeleteGO(&m_playerTurn);
}
bool CPlayer::Start()
{
	AddGO(enPlayerComponentPrio_Renderer, &m_playerRenderer, nullptr);
	AddGO(enPlayerComponentPrio_Animator, &m_playerAnimator, nullptr);
	AddGO(enPlayerComponentPrio_StateMachine, &m_playerStateMachine, nullptr);
	AddGO(enPlayerComponentPrio_Move, &m_playerMove, nullptr);
	AddGO(enPlayerComponentPrio_Turn, &m_playerTurn, nullptr);
	
	return true;
}

void CPlayer::Update()
{
	//回転行列から前方方向を計算する。
	CMatrix mRotation;
	mRotation.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRotation.m[2][0];
	m_forward.y = mRotation.m[2][1];
	m_forward.z = mRotation.m[2][2];

	//XZ平面での前方方向を計算。
	m_forwardXZ = m_forward;
	m_forwardXZ.y = 0.0f;
	m_forwardXZ.Normalize();

	//エフェクトテスト
	if (Pad(0).IsTrigger(enButtonX)) {
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/test.efk");
		CVector3 pos = m_position;
		pos.y += UnitM(0.5f);
		effect->SetPosition(pos);
		effect->SetRotation(m_rotation);
		effect->SetScale({ 4.0f, 4.0f, 4.0f });
	}
	if (Pad(0).IsTrigger(enButtonY)) {
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/test2.efk");
		CVector3 pos = m_position;
		pos.y += UnitM(0.5f);
		effect->SetPosition(pos);
		effect->SetRotation(m_rotation);
		effect->SetScale({ 4.0f, 4.0f, 4.0f });
	}
	if (Pad(0).IsTrigger(enButtonB)) {
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/test3.efk");
		CVector3 pos = m_position;
		pos.y += UnitM(0.01f);
		effect->SetPosition(pos);
		effect->SetScale({ 4.0f, 4.0f, 4.0f });
	}
	if (Pad(0).IsTrigger(enButtonUp)) {
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/test4.efk");
		CVector3 pos = m_position;
		pos.y += UnitM(0.1f);
		effect->SetPosition(pos);
		effect->SetScale({ 5.0f, 5.0f, 5.0f });
	}
}

void CPlayer::Render(CRenderContext& rc)
{
	(void)rc;
}