/*!
 * @brief	プレイヤー
 */

#pragma once

#include "tkEngine/character/tkCharacterController.h"
class Game;
class Player : public IGameObject {
public:

	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	 *@brief	ゲームオーバーを通血。
	 */
	void NotifyGameOver();
	/*!
	 *@brief	リスタートを通知。
	 */
	void NotifyRestart();
	/*!
	 *@brief	ゲームクリアー。
	 */
	void NotifyGameClear();
	/*!
	 *@brief	クリアできる？
	 */
	bool IsPossibleClear() const
	{
		if (m_state != enState_Jump
			&& m_state != enState_GameOver
			&& m_state != enState_WaitStartGameClear
			&& m_state != enState_GameClear)
		{
			return true;
		}
		return false;
	}
	/*!
	*@brief	スキンモデルの取得。
	*/
	CSkinModel& GetModel()
	{
		return m_modelRender->GetSkinModel();
	}
private:
	void UpdateFSM();
	void Move();
	void Turn();
	void AnimationController();
	
private:
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_run,
		enAnimationClip_jump,
		enAnimationClip_damage,
		enAnimationClip_KneelDown,
		enAnimationClip_Clear,
		enAnimationClip_num,
	};
	enum EnState {
		enState_Idle,
		enState_Run,
		enState_Jump,
		enState_GameOver,
		enState_WaitStartGameClear,
		enState_GameClear,
	};
	prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラ。
	CVector3 m_position = CVector3::Zero;			//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CAnimationClip m_animClip[enAnimationClip_num];
	
	prefab::CDirectionLight* m_charaLight = nullptr;	//!<キャラクタ用のライト。
	CCharacterController m_charaCon;		//キャラクタコントローラ。
	CVector3 m_moveSpeed = CVector3::Zero;
	CShaderResourceView m_normalMap;		//法線マップ。
	CShaderResourceView m_specMap;			//スペキュラマップ。
	CVector3 m_forward;						//プレイヤーの前方。
	EnState m_state = enState_Idle;
	Game* m_game = nullptr;
	float m_moveSpeedWhenStartJump;			//ジャンプを開始したときの移動速度。
	float m_timer = 0.0f;
};