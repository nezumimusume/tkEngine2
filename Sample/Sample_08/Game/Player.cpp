#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}
bool Player::Start()
{
	m_skinModelData.Load(L"modelData/Thethief_H.cmo");
	m_skinModel.Init(m_skinModelData);
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		20.0,			//半径。 
		50.0f,			//高さ。
		m_position		//初期位置。
	);
	//アニメーションを初期化。
	InitAnimation();
	return true;
}
void Player::InitAnimation()
{
	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/idle.tka");
	m_animationClip[enAnimationClip_run].Load(L"animData/run.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/walk.tka");
	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	//アニメーションを初期化。
	m_animation.Init(m_skinModel, m_animationClip, enAnimationClip_num);
	//待機アニメーションを流す。
	m_animation.Play(enAnimationClip_idle);
}

void Player::Update()
{
	//このフレームの移動量を求める。
	m_moveSpeed.x = Pad(0).GetLStickXF() * -150.0f;
	m_moveSpeed.z = Pad(0).GetLStickYF() * -150.0f;
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	
	//ワールド行列を更新。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisX, CMath::PI);
	m_skinModel.Update(m_position, qRot, CVector3::One);
}
void Player::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc);
}
