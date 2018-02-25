#pragma once
#include "GameActor.h"
#include "Command.h"
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:
	GameActor* m_gameActor = nullptr;
	Command_Run			m_commandRun;
	Command_Walk		m_commandWalk;
	Command_Idle		m_commandIdle;
	Command_TurnLeft	m_commandTurnLeft;
	Command_TurnRight	m_commandTurnRight;
};

