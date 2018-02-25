#include "stdafx.h"
#include "GameCamera.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");
	//カメラのニアクリップとファークリップを設定する。
	MainCamera().SetNear(1.0f);
	MainCamera().SetFar(10000.0f);
	m_springCamera.Init(MainCamera(), 1200.0f, false, 20.0f);
	return true;
}
void GameCamera::Update()
{
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
	if (m_isDebugCamera) {
		float debugCameraSpeed = 8.0f;
		if (Pad(0).IsPress(enButtonUp) ){
			if (Pad(0).IsPress(enButtonRB2)) {
				MainCamera().m_debugCameraPosition.y += debugCameraSpeed;
				MainCamera().m_debugCameraTarget.y += debugCameraSpeed;
			}
			else {
				MainCamera().m_debugCameraPosition += MainCamera().GetForward() * debugCameraSpeed;
				MainCamera().m_debugCameraTarget += MainCamera().GetForward() * debugCameraSpeed;
			}
		}
		if (Pad(0).IsPress(enButtonDown)) {
			if (Pad(0).IsPress(enButtonRB2)) {
				MainCamera().m_debugCameraPosition.y -= debugCameraSpeed;
				MainCamera().m_debugCameraTarget.y -= debugCameraSpeed;
			}
			else {
				MainCamera().m_debugCameraPosition += MainCamera().GetForward() * -debugCameraSpeed;
				MainCamera().m_debugCameraTarget += MainCamera().GetForward() * -debugCameraSpeed;
			}
		}
		if (Pad(0).IsPress(enButtonRight)) {
			MainCamera().m_debugCameraPosition += MainCamera().GetRight() * debugCameraSpeed;
			MainCamera().m_debugCameraTarget += MainCamera().GetRight() * debugCameraSpeed;
		}
		if (Pad(0).IsPress(enButtonLeft)) {
			MainCamera().m_debugCameraPosition += MainCamera().GetRight() * -debugCameraSpeed;
			MainCamera().m_debugCameraTarget += MainCamera().GetRight() * -debugCameraSpeed;
		}
		MainCamera().Update();
		return;
	}
	else {
		if (Pad(0).IsPress(enButtonStart)
			&& Pad(0).IsPress(enButtonDown)) {
			MainCamera().m_isDebugCamera = true;
			MainCamera().m_debugCameraPosition = MainCamera().GetPosition();
			MainCamera().m_debugCameraTarget = MainCamera().GetTarget();
			m_isDebugCamera = true;
		}
	}
#endif
	//カメラを更新。
	//注視点を計算する。
	CVector3 target = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;
	//視点を計算する。
	CVector3 toPos = { 0.0f, 350.0f, 550.0f };
	CVector3 pos = target + toPos;	//キャラクタを斜め上から見ているような視点にする。
	//メインカメラに注視点と視点を設定する。
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(pos);
	m_springCamera.Update();
}
void GameCamera::NotifyGameOver()
{
	//バネカメラをリフレッシュ。
	m_springCamera.Refresh();
	//バネカメラを非アクティブにする。
	m_isActive = false;
}
void GameCamera::NotifyRestart()
{
	//バネカメラをリフレッシュ。
	m_springCamera.Refresh();
	//バネカメラを非アクティブにする。
	m_isActive = true;
}
void GameCamera::NotifyGameClear()
{
	//バネカメラをリフレッシュ。
	m_springCamera.Refresh();
	//バネカメラを非アクティブにする。
	m_isActive = false;
}