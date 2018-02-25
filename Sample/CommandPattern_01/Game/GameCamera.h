#pragma once

#include "GameActor.h"
//ばねカメラを使用したい場合はtkSpringCamera.hをインクルードする必要がある。
#include "tkEngine/camera/tkSpringCamera.h"

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
	GameActor* m_actor;	//プレイヤー。
	CVector3 m_toCameraPos;	//
	CSpringCamera m_springCamera; //ばねカメラ。
};

