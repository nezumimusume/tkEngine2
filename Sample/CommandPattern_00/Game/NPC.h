#pragma once
#include "Command.h"

class GameActor;
class NPC : public IGameObject
{
public:
	NPC();
	~NPC();
	bool Start();
	void Update() override
	{
	}
private:
	GameActor* m_actor = nullptr;	//アクタ。
	//コマンド。
	Command_Run			m_commandRun;
	Command_Walk		m_commandWalk;
	Command_Idle		m_commandIdle;
	Command_TurnLeft	m_commandTurnLeft;
	Command_TurnRight	m_commandTurnRight;
};

