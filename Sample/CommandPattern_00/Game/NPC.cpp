#include "stdafx.h"
#include "NPC.h"


NPC::NPC()
{
}


NPC::~NPC()
{
}
bool NPC::Start()
{
	//暗殺者 Aを取得。
	m_actor = FindGO<GameActor>("暗殺者");
	//アクターにコマンドを積んでいく。
	for (int i = 0; i < 180; i++) {		//180フレーム走る。
		m_actor->AddCommand(&m_commandRun);
	}
	for (int i = 0; i < 20; i++) {		//左に旋回。
		m_actor->AddCommand(&m_commandTurnLeft);
	}
	for (int i = 0; i < 60; i++) {		//60フレーム歩く。
		m_actor->AddCommand(&m_commandWalk);
	}
	for (int i = 0; i < 40; i++) {		//右に旋回。
		m_actor->AddCommand(&m_commandTurnRight);
	}
	for (int i = 0; i < 30; i++) {		//30フレーム歩く。
		m_actor->AddCommand(&m_commandWalk);
	}
	//実習　コマンドを追加してNPCを動かしてみる。。
	//待機で終わり。
	m_actor->AddCommand(&m_commandIdle);
	return true;
}