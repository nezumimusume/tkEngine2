/*!
 * @brief	エネミー。
 */

#pragma once

#include "tkEngine/graphics/tkSkinModelRender.h"

class Enemy : public IGameObject{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Enemy();
	/*!
	 * @brief	デストラクタ。ｓ
	 */
	~Enemy();
	/*!
	 * @brief	スタート。
	 */
	bool Start() override;
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	
	/*!
	 *@brief	座標を設定。
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enANimationClip_Num,
	};
	EnAnimationClip m_currentAnimClip = enAnimationClip_Idle;	//!<現在再生中のアニメーションクリップ。
	CAnimationClip m_animClips[enANimationClip_Num];			//!<アニメーションクリップ；。
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//!<スキンモデルレンダラー。
	CVector3		m_position;									//!<座標。
	CQuaternion		m_rotation = CQuaternion::Identity;			//!<回転。
};