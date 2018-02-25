#pragma once
//キャラクターコントローラーを使用する場合は、下記のヘッダーファイルのインクルードが必要。
#include "tkEngine/character/tkCharacterController.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Render(CRenderContext& rc);
	bool Start();

	//アニメーションを初期化。
	void InitAnimation();
	//アニメーション制御。
	void AnimationControl();
	////////////////////////////////////
	// メンバ変数とか
	////////////////////////////////////
	enum EnAnimationClip {
		enAnimationClip_idle,	//待機アニメーション。
		enAnimationClip_run,	//走りアニメーション。
		enAnimationClip_walk,	//歩きアニメーション。
		enAnimationClip_num,	//アニメーションクリップの総数。
	};
	CAnimationClip m_animationClip[enAnimationClip_num];	//アニメーションクリップ。
	CAnimation m_animation;			//アニメーション。
	CSkinModelData m_skinModelData;	//スキンモデルデータ。
	CSkinModel m_skinModel;			//スキンモデル。
	CVector3 m_position = CVector3::Zero;	//座標。

	CCharacterController m_charaCon;		//キャラクターコントローラー。
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
};

