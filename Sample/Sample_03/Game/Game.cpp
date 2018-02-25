#include "stdafx.h"
#include "Game.h"


void Game::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
bool Game::Start()
{
	
	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, 35.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 35.0f, 80.0f });
	MainCamera().Update();
	

	//アニメーションクリップのロード。
	m_animClips[enAnimationClip_idle].Load(L"animData/idle.tka");
	m_animClips[enAnimationClip_run].Load(L"animData/run.tka");
	m_animClips[enAnimationClip_walk].Load(L"animData/walk.tka");
	//ループフラグを設定する。<-走りアニメーションはループフラグを設定していないので
	//ワンショット再生で停止する。
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_walk].SetLoopFlag(true);

	//スキンモデルレンダラーを作成。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Thethief_H.cmo", m_animClips, enAnimationClip_Num);
	m_skinModelRender->PlayAnimation(enAnimationClip_run);
	
	return true;
}
void Game::Update()
{
	//3dsMax上で90度回されているので、こっちで回す。
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisX, 180.0f);
	//Aボタンが押されたら待機モーションを再生する。
	if (Pad(0).IsTrigger(enButtonA)) {
		//Play関数の第二引数は補完時間。
		//待機モーションの切り替えは補完時間を設定していないので、アニメーションがパキっと変わる。
		m_skinModelRender->PlayAnimation(enAnimationClip_idle);
	}
	//Bボタンが押されたら走りアニメーションを再生する。
	else if (Pad(0).IsTrigger(enButtonB)) {
		m_skinModelRender->PlayAnimation(enAnimationClip_run, 0.2f);
	}
	//Yボタンが押されたら歩きアニメーションを再生する。
	else if (Pad(0).IsTrigger(enButtonY)) {
		m_skinModelRender->PlayAnimation(enAnimationClip_walk, 0.2f);
	}
	
	//回転を設定。
	m_skinModelRender->SetRotation(qRot);
}
