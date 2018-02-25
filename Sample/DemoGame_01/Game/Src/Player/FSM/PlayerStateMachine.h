/*!
 * @brief	プレイヤーのステートマシーン。
 */

#pragma once

#include "Player/FSM/PlayerState.h"
#include "Player/PlayerConst.h"

class CPlayerStateMachine : public IGameObject{
public:
	typedef std::function<void(CPlayerConst::EnState)> OnChangeState;
	CPlayerStateMachine(CPlayer* pl) :
		m_idleState(pl, this),
		m_runState(pl, this),
		m_walkState(pl, this),
		m_jumpState(pl, this)
	{
	}
	bool Start() override final;
	void Update() override final
	{
	}
	/*!
	* @brief	ステートの切り替え。
	*@param[in]	nextState	次のステート。
	*/
	void ChangeState(CPlayerConst::EnState nextState);
	/*!
	 *@brief	移動できるか判定。
	 */
	bool IsMove() const
	{
		if (m_currentState == nullptr) {
			return false;
		}
		return m_currentState->IsMove();
	}
	/*!
	 *@brief	重力の影響を受けるか判定。
	 */
	bool IsApplyGravity() const
	{
		if (m_currentState == nullptr) {
			return false;
		}
		return m_currentState->IsApplyGravity();
	}
	/*!
	 *@brief	現在の状態を取得。
	 */
	CPlayerConst::EnState GetCurrentState() const
	{
		return m_state;
	}
	/*!
	 *@brief	ステート切り替えのリスナーを登録。
	 */
	void AddChangeStateListner(OnChangeState listner)
	{
		m_changeStateListener.push_back(listner);
	}
private:
	
	CPlayerConst::EnState m_state = CPlayerConst::enState_Invalid;
	IPlayerState* m_currentState = nullptr;
	CPlayerStateIdle m_idleState;
	CPlayerStateWalk m_walkState;
	CPlayerStateRun m_runState;
	CPlayerStateJump m_jumpState;
	
	std::vector<OnChangeState>	m_changeStateListener;	//!<ステート切り替えリスナー。
};