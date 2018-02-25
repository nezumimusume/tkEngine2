#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
Enemy::Enemy() :
	m_pathMoveLoop(this, m_movePath)
{
}


Enemy::~Enemy()
{
}
bool Enemy::Start()
{
	m_skinModelData.Load(L"modelData/enemy.cmo");
	m_skinModel.Init(m_skinModelData);
	m_skinModel.SetShadowCasterFlag(true);
	m_skinModel.SetShadowReceiverFlag(true);

	m_animClip[enAnimationClip_idle].Load(L"animData/enemy/idle.tka");
	m_animClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animClip[enAnimationClip_run].Load(L"animData/enemy/run.tka");
	m_animClip[enAnimationClip_run].SetLoopFlag(true);

	m_animation.Init(m_skinModel, m_animClip, enAnimationClip_num);
	m_animation.Play(enAnimationClip_idle);

	m_charaCon.Init(20.0f, 100.0f, m_position);
	m_player = FindGO<Player>("Player");
	m_game = FindGO<Game>("Game");
	return true;
}

void Enemy::NotifyRestart()
{
	m_isFindPlayer = false;
}
void Enemy::SearchPlayer()
{
	CVector3 toPlayer = m_player->GetPosition() - m_position;
	if (toPlayer.Length() < 400.0f) {
		//視野角を判定。
		toPlayer.Normalize();

		float angle = acosf(toPlayer.Dot(m_forward));
		if (fabsf(angle) < CMath::PI * 0.25f) {
			//視野角45度以内。
			//ゲームにゲームオーバーを通知。
			Game* game = FindGO<Game>("Game");
			game->NotifyGameOver();
			m_isFindPlayer = true;
		}
	}
}
void Enemy::Update()
{
	if (!m_game->IsGameOver() && !m_game->IsGameClear()) {
		SearchPlayer();
		m_pathMoveLoop.Update();
	}
	const CVector3 scale = { 3.0f, 3.0f, 3.0f };
	m_skinModel.Update(m_position, m_rotation, scale);
	const CMatrix& mWorld = m_skinModel.GetWorldMatrix();

	CMatrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
void Enemy::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
