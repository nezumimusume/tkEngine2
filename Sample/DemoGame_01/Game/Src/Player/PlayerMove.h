/*!
 * @brief	プレイヤーの移動処理。
 */

#pragma once

#include "tkEngine/character/tkCharacterController.h"

class CPlayer;
class CGameCamera;

class CPlayerMove : public IGameObject{
public:
	CPlayerMove( CPlayer* pl );
	~CPlayerMove();
	bool Start() override final;
	void Update() override final;
	/*!
	 *@brief	移動速度の取得。
	 */
	CVector3 GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*!
	 *@brief	速度を加える。
	 */
	void AddMoveSpeed(CVector3 speed)
	{
		m_addMoveSpeed += speed;
	}
	/*!
	 *@brief	ジャンプ中であることを通知
	 */
	void Jump()
	{
		m_charaCon.Jump();
	}
	/*!
	 *@brief	プレイヤーが地面にいるかどうか判定。
	 */
	bool IsOnGround() const
	{
		return m_charaCon.IsOnGround();
	}
	/*!
	 *@brief	プレイヤーの状態が切り替わったときに呼ばれるコールバック関数。
	 */
	void OnChangeState(CPlayerConst::EnState nextState);
private:
	CCharacterController m_charaCon;		//キャラクターコントローラ。
	CPlayer* m_player = nullptr;
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
	CVector3 m_addMoveSpeed = CVector3::Zero;	//加算される移動速度。
	CGameCamera* m_gameCamera = nullptr;
	float m_accelTime = 0.0f;
};