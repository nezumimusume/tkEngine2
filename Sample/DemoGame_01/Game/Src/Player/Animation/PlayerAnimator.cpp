/*!
 * @brief	プレイヤーのアニメータ。
 */

#include "stdafx.h"
#include "Player/Player.h"
#include "Player/PlayerRenderer.h"
#include "Player/Animation/PlayerAnimator.h"
#include "tkEngine/graphics/effect/tkEffect.h"

bool CPlayerJumpAnimCtr::Start()
{
	m_animator.PlayAnimation(CPlayerAnimator::enAnimationClip_Jump, 0.2f);
	//@todo FootIK用のパラメータを設定する。
	//プレイヤーにジャンプ中であることを通知する。
	m_player.Jump();
	m_state = enState_PlayJump;
	m_freezeBoneTranslation = CVector3::Zero;
	m_timer = 0.0f;

	return true;
}
void CPlayerJumpAnimCtr::Update()
{
	switch (m_state) {
	case enState_PlayJump: {
		m_timer += GameTime().GetFrameDeltaTime();
		if (m_timer < 0.2f) {
			//ジャンプモーションが開始されて、2秒間くらいはずっとジャンプ通知をしておく。
			m_player.Jump();
		}
		
		if (m_player.IsOnGround()) {
			//地面に降り立った。
			m_animator.PlayAnimation(CPlayerAnimator::enAnimationClip_JumpEnd, 0.1f);
			m_state = enState_PlayJumpEnd;
		}else {
			//ジャンプモーション中はfootstepのZ方向の移動量をY方向の移動速度にする。
			CVector3 freezeBoneTranslate = m_animator.GetFreezeBoneTranslate();
			//前のフレームとの差分を計算する。
			CVector3 moveSpeed = freezeBoneTranslate - m_freezeBoneTranslation;
			moveSpeed *= (1.0f / GameTime().GetFrameDeltaTime());	//速度をcm/秒に変換する。
			moveSpeed.x = 0.0f;
			moveSpeed.y = -moveSpeed.z;
			moveSpeed.z = 0.0f;
			m_player.AddMoveSpeed(moveSpeed);
			m_freezeBoneTranslation = freezeBoneTranslate;
		}
	}break;
	case enState_PlayJumpEnd:
		if (m_animator.IsPlaying() == false) {
			//終わり。
		}
		break;
	}
}


void CPlayerJumpAnimCtr::OnDestroy()
{
}


void CPlayerAnimator::OnInvokeAnimationEvent(
	const wchar_t* animClipName, 
	const wchar_t* eventName
)
{
	if (wcscmp(eventName, L"FootStepSE") == 0) {
		auto soundSource = NewGO<prefab::CSoundSource>(0);
		soundSource->Init("sound/Footstep_00.wav");
		soundSource->Play(false);
	}
}a
bool CPlayerAnimator::Start()
{
	//ステート切り換えのリスナーを登録する。
	m_player->AddChangeStateListner([&](auto nextState) {
		OnChangeState(nextState);
	});
	int boneNo = m_player->FindBoneID(L"thief_Bip01_Footsteps");

	m_animClips[enAnimationClip_Idle].Load(L"animData/player/idle.tka");
	m_animClips[enAnimationClip_Idle].SetLoopFlag(true);	
	
	m_animClips[enAnimationClip_Walk].Load(L"animData/player/walk.tka");
	m_animClips[enAnimationClip_Walk].SetLoopFlag(true);

	m_animClips[enAnimationClip_Run].Load(L"animData/player/run.tka");
	m_animClips[enAnimationClip_Run].SetLoopFlag(true);

	m_animClips[enAnimationClip_Jump].Load(L"animData/player/jump.tka");
	m_animClips[enAnimationClip_Jump].SetLoopFlag(false);
	m_animClips[enAnimationClip_Jump].SetFreezeBoneInfo(boneNo, true, true, true);	//Z軸の移動をフリーズ。

	m_animClips[enAnimationClip_JumpEnd].Load(L"animData/player/jump_end.tka");
	m_animClips[enAnimationClip_JumpEnd].SetLoopFlag(false);

	m_animation.Init(m_player->GetPlayerRenderer().GetSkinModel(), m_animClips, enANimationClip_Num);
	m_currentAnimClip = enAnimationClip_Idle;
	m_animation.Play(enAnimationClip_Idle);

	m_animation.AddAnimationEventListener([&](auto clipName, auto eventName) {
		OnInvokeAnimationEvent(clipName, eventName);
	});
	//FootIKのパラメータを作成する。
	/*CAnimationFootIK::SFootIKParam footIKParam;
	footIKParam.footBoneName_0 = L"thief_Bip01_L_Toe0";
	footIKParam.footBoneName_1 = L"thief_Bip01_R_Toe0";
	footIKParam.rootBoneName = L"thief_Bip01_Spine";
	footIKParam.footCapsuleColliderHeight_0 = UnitCM(2.0f);
	footIKParam.footCapsuleColliderHeight_1 = UnitCM(2.0f);
	footIKParam.footCapsuleColliderRadius_0 = UnitCM(1.0f);
	footIKParam.footCapsuleColliderRadius_0 = UnitCM(1.0f);
	m_animation.EnableFootIK(footIKParam);*/

	return true;
}
void CPlayerAnimator::OnChangeState(CPlayerConst::EnState nextState)
{
	//登録されているかもしれないので破棄する。
	DeleteGO(&m_jumpAnimCtr);
	if (nextState == CPlayerConst::enState_Idle) {		
		PlayAnimation(enAnimationClip_Idle, 0.2f);
	}
	else if (nextState == CPlayerConst::enState_Run)
	{
		PlayAnimation(enAnimationClip_Run, 0.2f);
	}
	else if (nextState == CPlayerConst::enState_Walk) {
		PlayAnimation(enAnimationClip_Walk, 0.2f);
	}
	else if (nextState == CPlayerConst::enState_Jump) {
		//ジャンプのアニメーション制御を登録する。
		AddGO(GetPriority(), &m_jumpAnimCtr);
	}
}
void CPlayerAnimator::Update()
{	
}