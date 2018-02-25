/*!
 * @brief	プレイヤーのアニメーター。
 *@details
 * プレイヤーのアニメーションを制御します。
 */

#pragma once

#include "Player/PlayerConst.h"

class CPlayer;
class CPlayerAnimator;

/*!
 *@brief	ジャンプアニメーション制御。
 */
class CPlayerJumpAnimCtr : public IGameObject {
public:
	CPlayerJumpAnimCtr(CPlayer& pl, CPlayerAnimator& animator) :
		m_player(pl),
		m_animator(animator) {}
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;
private:
	enum EnState {
		enState_PlayJump,
		enState_PlayJumpEnd,
	};
	EnState		m_state = enState_PlayJump;
	CPlayerAnimator& m_animator;	//!<アニメータ。
	CVector3 m_freezeBoneTranslation = CVector3::Zero;
	CPlayer& m_player;
	float m_timer = 0.0f;
};
/*!
 * @brief	プレイヤーのアニメーター。
 */
class CPlayerAnimator : public IGameObject{
public:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_JumpEnd,
		enANimationClip_Num,
	};
	CPlayerAnimator(CPlayer* player) :
		m_jumpAnimCtr(*player, *this)
	{
		m_player = player;
	}
	~CPlayerAnimator()
	{
	}
	bool Start() override;
	void Update() override;
	/*!
	 *@brief	再生中のアニメーションクリップを取得。
	 */
	EnAnimationClip GetCurrentAnimationClip() const
	{
		return m_currentAnimClip;
	}
	/*!
	 *@brief	アニメーションが再生中か判定。
	 */
	bool IsPlaying() const
	{
		return m_skinModelRender->IsPlayingAnimation();
	}
	
	void PlayAnimation(EnAnimationClip animClip, float interpolateTime)
	{
		m_currentAnimClip = animClip;
		m_skinModelRender->PlayAnimation(animClip, interpolateTime);
	}
	/*!
	 *@brief	状態が変更されたときのコールバック関数。
	 */
	void OnChangeState(CPlayerConst::EnState nextState);
	/*!
	*@brief	フリーズさせているボーンの平行移動量を取得。
	*@details
	* 最終ポーズのアニメーションだけ。
	*/
	CVector3 GetFreezeBoneTranslate() const
	{
		return m_skinModelRender->GetFreezeBoneTranslate();
	}
	/*!
	*@brief	アニメーションイベントが起きた時に呼ばれる処理。
	*/
	void OnInvokeAnimationEvent(const wchar_t* animClipName, const wchar_t* eventName);
private:
	EnAnimationClip m_currentAnimClip = enAnimationClip_Idle;		//!<現在再生中のアニメーションクリップ。
	CAnimationClip m_animClips[enANimationClip_Num];	//!<アニメーションクリップ；。
	CPlayer* m_player = nullptr;						//!<プレイヤーレンダラー。
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CPlayerJumpAnimCtr m_jumpAnimCtr;
};