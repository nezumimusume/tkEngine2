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
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	//アニメーションクリップのロード。
	m_animClips[enAnimationClip_Idle].Load(L"animData/enemy/idle.tka");
	m_animClips[enAnimationClip_Idle].SetLoopFlag(true);
	//スキンモデルの初期化。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init( L"modelData/enemy_00.cmo", m_animClips, enANimationClip_Num);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);
	m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
	
	return true;
}

void Enemy::Update()
{
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetScale({ 4.0f, 4.0f, 4.0f });
}
