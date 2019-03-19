#include "stdafx.h"
#include "Player.h"

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
bool Player::Start()
{
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		20.0,			//半径。 
		50.0f,			//高さ。
		m_position		//初期位置。
	);
	
	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/idle.tka");
	m_animationClip[enAnimationClip_run].Load(L"animData/run.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/walk.tka");
	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Thethief_H.cmo", m_animationClip, enAnimationClip_num);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);
	return true;
}
void Player::Update()
{
	//このフレームの移動量を求める。
	m_moveSpeed.x = Pad(0).GetLStickXF() * -150.0f;
	m_moveSpeed.z = Pad(0).GetLStickYF() * -150.0f;
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute( m_moveSpeed);
	
	//座標と回転を設定。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisX, CMath::PI);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(qRot);
}

