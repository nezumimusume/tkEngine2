#pragma once

#include "tkEngine/camera/tkSpringCamera.h"

class Player;
class Enemy;
class GameOverCamera : public IGameObject
{
public:
	GameOverCamera();
	~GameOverCamera();
	void Init(const Player& player, const Enemy& enemy);
	bool Start() override;
	void Update() override;
private:
	CSpringCamera m_springCamera;
	const Player* m_player = nullptr;
	const Enemy* m_enemy =  nullptr;
};

