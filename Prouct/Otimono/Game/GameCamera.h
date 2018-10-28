#pragma once
#include "tkEngine/camera/tkSpringCamera.h"
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	void Update() override;
private:
	CSpringCamera m_springCamera;	//バネカメラ。
	Player* m_player = nullptr;
};

