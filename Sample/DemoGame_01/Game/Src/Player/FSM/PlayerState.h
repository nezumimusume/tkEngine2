/*!
 * @brief	プレイヤーのステート。
 */

#pragma once

#include "Player/FSM/IPlayerState.h"

/*!
 * @brief	待機ステート。
 */
class CPlayerStateIdle : public IPlayerState{
public:
	CPlayerStateIdle(CPlayer* pl, CPlayerStateMachine* psm) :
		IPlayerState(pl, psm)
	{
	}
	virtual ~CPlayerStateIdle()
	{
	}
	void Update() override final;
private:
	
};

/*!
 *@brief	歩きステート。
 */
class CPlayerStateWalk : public IPlayerState {
public:
	CPlayerStateWalk(CPlayer* pl, CPlayerStateMachine* psm) :
		IPlayerState(pl, psm) {}
	void Update() override final;
};
/*!
 *@brief	走りステート。
 */
class CPlayerStateRun : public IPlayerState {
public:
	CPlayerStateRun(CPlayer* pl, CPlayerStateMachine* psm) :
		IPlayerState(pl, psm) {}
	void Update() override final;
};
/*!
*@brief	ジャンプステート。
*/
class CPlayerStateJump : public IPlayerState {
public:
	CPlayerStateJump(CPlayer* pl, CPlayerStateMachine* psm) :
		IPlayerState(pl, psm) {}
	bool Start() override final
	{
		m_waitTimer = 2;
		
		return true;
	}
	void Update() override final;
	/*!
	* @brief	移動できるか判定
	*/
	bool IsMove() const override;
	/*!
	 *@brief	重力の影響を受けるか判定。
	 */
	bool IsApplyGravity() const override;
private:
	int m_waitTimer = 1;
};