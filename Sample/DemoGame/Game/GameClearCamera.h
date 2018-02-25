#pragma once


#include "tkEngine/camera/tkSpringCamera.h"

class Player;

class GameClearCamera : public IGameObject
{
public:
	GameClearCamera();
	~GameClearCamera();
	bool Start() override;
	void Update() override;
private:
	CSpringCamera m_springCamera;
	const Player* m_player = nullptr;
};

