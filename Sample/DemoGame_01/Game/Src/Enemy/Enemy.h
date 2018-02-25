/*!
 * @brief	エネミー。
 */

#pragma once

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
	 * @brief	描画
	 *@param[in]	rc		レンダリングコンテキスト。
	 */
	void Render(CRenderContext& rc) override;
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
	EnAnimationClip m_currentAnimClip = enAnimationClip_Idle;		//!<現在再生中のアニメーションクリップ。
	CAnimationClip m_animClips[enANimationClip_Num];	//!<アニメーションクリップ；。
	CAnimation m_animation;								//!<アニメーション。
	CSkinModelData	m_skinModelData;	//!<スキンモデルデータ。
	CSkinModel		m_skinModel;		//!<スキンモデル。
	CVector3		m_position;			//!<座標。
	CQuaternion		m_rotation;			//!<回転。
};