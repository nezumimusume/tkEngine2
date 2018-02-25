#pragma once

#include "Player.h"
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
	Player* m_player;	//プレイヤー。
	CVector3 m_toCameraPos;	//
	CSpringCamera m_springCamera; //ばねカメラ。
};

