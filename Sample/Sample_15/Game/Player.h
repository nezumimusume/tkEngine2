#pragma once
//キャラクターコントローラーを使用する場合は、下記のヘッダーファイルのインクルードが必要。
#include "tkEngine/character/tkCharacterController.h"

class Player : public IGameObject
{
public:
	
	void OnDestroy() override;
	void Update() override;
	bool Start() override;	
	//移動処理。
	void Move();
	void Turn();
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
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position = CVector3::Zero;	//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CCharacterController m_charaCon;		//キャラクターコントローラー。
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
	
	CShaderResourceView	m_normalMapSRV;			//!<法線マップのSRV。
	CShaderResourceView m_specularMapSRV;		//!<スペキュラマップのSRV。
	CShaderResourceView	m_wnormalMapSRV;		//!<武器の法線マップのSRV。
	CShaderResourceView m_wspecularMapSRV;		//!<武器のスペキュラマップのSRV。
};

