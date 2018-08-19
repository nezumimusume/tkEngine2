#pragma once

#include "Path.h"
#include "EnemyPathMoveLoop.h"
#include "tkEngine/character/tkCharacterController.h"

class Player;
class Game;
class Enemy : public IGameObject
{
public:
	const float MOVE_SPEED = 150.0f;
	const float RADIUS = 20.0f;
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_run,
		enAnimationClip_num,
	};
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	
	/*!
	 *@brief	座標の取得。
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/*!
	 *@brief	座標の設定。
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/*!
	 *@brief	初期化。
	 */
	void Init(const wchar_t* movePathFilePath)
	{
		m_movePath.Load(movePathFilePath);
	}
	/*!
	 *@brief	回転を設定。
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/*!
	 *@brief	アニメーションを再生。
	 *@param[in]	animNo		アニメーション番号。
	 */
	void PlayAnimation(EnAnimationClip animNo)
	{
		m_modelRender->PlayAnimation(animNo);
	}
	void Move(float deltaTime, CVector3 move)
	{
		(void)deltaTime;
		m_position = m_charaCon.Execute(move);
	}
	/*!
	 *@brief	敵の前方向を取得。
	 */
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	/*!
	 *@brief	プレイヤーを発見したか判定。
	 */
	bool IsFindPlayer() const
	{
		return m_isFindPlayer;
	}
	/*!
	 *@brief	リスタートを通知。
	 */
	void NotifyRestart();
private:
	/*!
	 *@brief	プレイヤーを索敵
	 */
	void SearchPlayer();
private:
	
	prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラー。
	CAnimationClip m_animClip[enAnimationClip_num];	//!<アニメーションクリップ。
	CVector3 m_position = CVector3::Zero;			//!<座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//!<回転。
	Path m_movePath;								//!<移動パス。
	EnemyPathMoveLoop m_pathMoveLoop;				//!<パスのループ移動思考。
	CCharacterController m_charaCon;				//!<キャラクタコントローラ。
	CVector3 m_forward = CVector3::AxisZ;			//!<エネミーの前方方向。
	Player* m_player = nullptr;						//!<プレイヤー。
	Game* m_game = nullptr;
	bool m_isFindPlayer = false;					//!<プレイヤーを発見。
};

