/*!
 * @brief	プレイヤー関係の定数。
 */


#pragma once


class CPlayerConst{
public:
	static const float RUN_SPEED;		//プレイヤーの移動速度。
	/*!
	 * @brief	プレイヤーの状態。
	 */
	enum EnState {
		enState_Idle,		//!<待機ステート。
		enState_Walk,		//!<歩きステート。
		enState_Run,		//!<走りステート。
		enState_Jump,		//!<ジャンプステート。
		enState_Invalid,	//!<無効ステート。
	};
};