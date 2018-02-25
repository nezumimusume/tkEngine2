/*!
 * @brief	プレイヤーのステート
 */

#include "stdafx.h"
#include "Player/Player.h"
#include "Player/FSM/PlayerState.h"


namespace {
	const float WALK_THREDHOLD = 0.01f;							//!<歩き状態に遷移する速度の閾値。
	const float RUN_THREDHOLD = CPlayerConst::RUN_SPEED * 0.8f;	//!<走り状態に遷移する速度の閾値。
}
void CPlayerStateIdle::Update()
{
	CVector3 moveSpeed = m_player->GetMoveSpeed();
	float runThreadhold = RUN_THREDHOLD ;
	float moveSpeedScaler = moveSpeed.Length();
	if (Pad(0).IsPress(enButtonA)) {
		m_psm->ChangeState(CPlayerConst::enState_Jump);
	}else if (moveSpeedScaler > runThreadhold) {
		//走り状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Run);
	}
	else if (moveSpeedScaler > WALK_THREDHOLD) {
		//歩き状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Walk);
	}
}

void CPlayerStateWalk::Update()
{
	CVector3 moveSpeed = m_player->GetMoveSpeed();
	float runThreadhold = RUN_THREDHOLD;
	float moveSpeedScaler = moveSpeed.Length();
	
	if (Pad(0).IsPress(enButtonA)) {
		m_psm->ChangeState(CPlayerConst::enState_Jump);
	}else if (moveSpeedScaler > runThreadhold) {
		//走り状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Run);
	}
	else if (moveSpeedScaler < WALK_THREDHOLD) {
		//待機状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Idle);
	}
}

void CPlayerStateRun::Update()
{
	CVector3 moveSpeed = m_player->GetMoveSpeed();
	float runThreadhold = RUN_THREDHOLD;
	float moveSpeedScaler = moveSpeed.Length();

	if (Pad(0).IsPress(enButtonA)) {
		m_psm->ChangeState(CPlayerConst::enState_Jump);
	}else if (moveSpeedScaler < runThreadhold) {
		//歩き状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Walk);
	}
	else if (moveSpeedScaler < WALK_THREDHOLD) {
		//待機状態に遷移。
		m_psm->ChangeState(CPlayerConst::enState_Idle);
	}
}
void CPlayerStateJump::Update()
{
	const CPlayerAnimator& animator = m_player->GetAnimator();
	CPlayerAnimator::EnAnimationClip clip = animator.GetCurrentAnimationClip();
	if ( clip != CPlayerAnimator::enAnimationClip_Jump
			&& clip != CPlayerAnimator::enAnimationClip_JumpEnd 
	) {
		//ジャンプアニメーション以外が再生されている。。		
		CVector3 moveSpeed = m_player->GetMoveSpeed();
		float runThreadhold = RUN_THREDHOLD;
		float moveSpeedScaler = moveSpeed.Length();
		if (moveSpeedScaler > runThreadhold) {
			//走り状態に遷移。
			m_psm->ChangeState(CPlayerConst::enState_Run);
		}
		else if (moveSpeedScaler > WALK_THREDHOLD) {
			//歩き状態に遷移。
			m_psm->ChangeState(CPlayerConst::enState_Walk);
		}
		else {
			//待機状態に遷移。
			m_psm->ChangeState(CPlayerConst::enState_Idle);
		}
	}
	else if (clip == CPlayerAnimator::enAnimationClip_JumpEnd) {
		if (m_waitTimer == 0) {
			//着地モーションが再生中。
			CVector3 moveSpeed = m_player->GetMoveSpeed();
			float runThreadhold = RUN_THREDHOLD;
			float moveSpeedScaler = moveSpeed.Length();
			if (moveSpeedScaler > runThreadhold) {
				//走り状態に遷移。
				m_psm->ChangeState(CPlayerConst::enState_Run);
			}
			else if (moveSpeedScaler > WALK_THREDHOLD) {
				//歩き状態に遷移。
				m_psm->ChangeState(CPlayerConst::enState_Walk);
			}
			else if (animator.IsPlaying() == false) {
				//待機状態に遷移。
				m_psm->ChangeState(CPlayerConst::enState_Idle);
			}
		}
		m_waitTimer = max(m_waitTimer--, 0);

	}
}

bool CPlayerStateJump::IsMove() const
{
	const CPlayerAnimator& animator = m_player->GetAnimator();
	if (animator.GetCurrentAnimationClip() == CPlayerAnimator::enAnimationClip_JumpEnd && m_waitTimer > 0) {
		return false;
	}
	return true;
}
/*!
*@brief	重力の影響を受けるか判定。
*/
bool CPlayerStateJump::IsApplyGravity() const
{
	const CPlayerAnimator& animator = m_player->GetAnimator();
	if (animator.GetCurrentAnimationClip() == CPlayerAnimator::enAnimationClip_Jump
		&& animator.IsPlaying()
		) {
		//ジャンプ中のアニメーション再生中は重力の影響は受けない。
		return false;
	}
	return true;
}