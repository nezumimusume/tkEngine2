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
void Player::AnimationControl()
{
	//アニメーションを進める。
	m_animation.Update(GameTime().GetFrameDeltaTime());
}
void Player::Update()
{
	//このフレームの移動量を求める。
	m_moveSpeed.x = Pad(0).GetLStickXF() * -150.0f;
	m_moveSpeed.z = Pad(0).GetLStickYF() * -150.0f;
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
	if (Pad(0).IsTrigger(enButtonA) //Aボタンが押されたら
		&& m_charaCon.IsOnGround()  //かつ、地面に居たら
	) {
		//ジャンプする。
		m_moveSpeed.y = 400.0f;	//上方向に速度を設定して、
		m_charaCon.Jump();		//キャラクターコントローラーにジャンプしたことを通知する。
	}
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);

	//アニメーションコントロール。
	AnimationControl();
	//ワールド行列を更新。
	CQuaternion qBias;
	qBias.SetRotationDeg(CVector3::AxisX, 180.0f);	//3dsMaxで設定されているアニメーションでキャラが回転しているので、補正を入れる。
	m_skinModel.Update(m_position, qBias, CVector3::One);
}
void Player::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
