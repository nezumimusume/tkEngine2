/*!
 * @brief	エネミー。
 */

#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

bool Enemy::Start()
{
	//スキンモデルの初期化。
	m_skinModelData.Load(L"modelData/enemy_00.cmo");
	m_skinModel.Init(m_skinModelData);
	m_skinModel.SetShadowCasterFlag(true);
	m_skinModel.SetShadowReceiverFlag(true);
	//アニメーションの初期化。
	m_animClips[enAnimationClip_Idle].Load(L"animData/enemy/idle.tka");
	m_animClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animation.Init(m_skinModel, m_animClips, enANimationClip_Num);
	m_animation.Play(enAnimationClip_Idle);
	return true;
}

void Enemy::Update()
{
	m_skinModel.Update(m_position, CQuaternion::Identity, {4.0f, 4.0f, 4.0f});
}

void Enemy::Render(CRenderContext& rc) 
{
	m_skinModel.Draw(rc);
}