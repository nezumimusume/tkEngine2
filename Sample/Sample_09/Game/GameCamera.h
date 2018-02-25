#pragma once

#include "Player.h"

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
};

