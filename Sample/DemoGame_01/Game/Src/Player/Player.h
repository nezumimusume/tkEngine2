/*!
 * @brief	プレイヤー
 */

#pragma once

#include "Player/Animation/PlayerAnimator.h"
#include "Player/PlayerRenderer.h"
#include "Player/FSM/PlayerStateMachine.h"
#include "Player/PlayerMove.h"
#include "Player/PlayerTurn.h"

class CPlayer : public IGameObject {
public:
	/*!
	* @brief	プレイヤのコンポーネントのプライオリティ。
	*/
	enum EnPlayerComponentPrio {
		enPlayerComponentPrio_StateMachine,		//ステートマシーン。
		enPlayerComponentPrio_Move,				//移動処理。
		enPlayerComponentPrio_Turn,				//旋回処理。
		enPlayerComponentPrio_Animator,			//アニメータ。
		enPlayerComponentPrio_Renderer,			//レンダラー。

	};
	/*!
	 * @brief	コンストラクタ。
	 */
	CPlayer();
	/*!
	 * @brief	デストラクタ。
	 */
	~CPlayer();
	/*!
	* @brief	終了。
	*/
	void OnDestroy() override;
	/*!
	 * @brief	スタート。
	 */
	bool Start() override;
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	 * @brief	描画
	 *@param[in]	rc		レンダリングコンテキスト。
	 */
	void Render(CRenderContext& rc) override;
	/*!
	* @brief	座標を取得。
	*/
	CVector3 GetPosition() const
	{
		return m_position;
	}
	/*!
	* @brief	座標を設定。
	*@param[in]	pos	座標。
	*/
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/*!
	* @brief	回転の取得。
	*/
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
	/*!
	* @brief	回転の設定。
	*/
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/*!
	* @brief	プレイヤーレンダラの取得。
	*/
	CPlayerRenderer& GetPlayerRenderer() 
	{
		return m_playerRenderer;
	}
	/*!
	 *@brief	移動可能か判定。
	 */
	bool IsPossibleMove() const
	{
		return m_playerStateMachine.IsMove();
	}
	/*!
	 *@brief	移動速度を取得。
	 */
	CVector3 GetMoveSpeed() const
	{
		return m_playerMove.GetMoveSpeed();
	}
	/*!
	 *@brief	プレイヤーの前方方向を取得。
	 */
	CVector3 GetForward() const
	{
		return m_forward;
	}
	/*!
	 *@brief	プレイヤーのXZ平面での前方方向を取得。
	 */
	CVector3 GetForwardXZ() const
	{
		return m_forwardXZ;
	}
	/*!
	 *@brief	現在の状態を取得。
	 */
	CPlayerConst::EnState GetCurrentState() const
	{
		return m_playerStateMachine.GetCurrentState();
	}
	
	/*!
	*@brief	重力の影響を受けるか判定。
	*/
	bool IsApplyGravity() const
	{
		return m_playerStateMachine.IsApplyGravity();
	}
	/*!
	 *@brief	アニメーターを取得。
	 */
	const CPlayerAnimator& GetAnimator() const
	{
		return m_playerAnimator;
	}
	/*!
	 *@brief	ボーンの名前からボーンIDを検索。
	 */
	int FindBoneID(const wchar_t* boneName) const
	{
		return m_playerRenderer.GetSkinModelData().GetSkeleton().FindBoneID(boneName);
	}
	/*!
	*@brief	ボーンを取得。
	*/
	CBone* GetBone(int boneNo)
	{
		return m_playerRenderer.GetSkinModelData().GetSkeleton().GetBone(boneNo);
	}
	/*!
	*@brief	速度を加える。
	*/
	void AddMoveSpeed(CVector3 speed)
	{
		m_playerMove.AddMoveSpeed(speed);
	}
	/*!
	*@brief	ステート切り替えのリスナーを登録。
	*/
	void AddChangeStateListner(CPlayerStateMachine::OnChangeState listner)
	{
		m_playerStateMachine.AddChangeStateListner(listner);
	}
	/*!
	 *@brief	プレイヤーが地面にいるか判定。
	 */
	bool IsOnGround() const
	{
		return m_playerMove.IsOnGround();
	}
	/*!
	 *@brief	プレイヤーにジャンプ中であることを通知。
	 */
	void Jump()
	{
		m_playerMove.Jump();
	}
private:
	CPlayerRenderer m_playerRenderer;				//プレイヤレンダラ。
	CPlayerAnimator m_playerAnimator;				//アニメータ。
	CPlayerMove	m_playerMove;						//移動処理。
	CPlayerTurn m_playerTurn;						//旋回処理。
	CPlayerStateMachine m_playerStateMachine;		//ステートマシーン。
	CVector3 m_position = CVector3::Zero;			//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CVector3 m_forward = CVector3::Front;			//プレイヤーの前方方向。
	CVector3 m_forwardXZ = CVector3::Front;			//プレイヤーのXZ平面での前方方向。
};
