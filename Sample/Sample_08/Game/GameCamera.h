#pragma once

#include "Player.h"

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
private:
	Player* m_player;	//プレイヤー。
};

