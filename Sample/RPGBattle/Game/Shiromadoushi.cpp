#include "stdafx.h"
#include "Shiromadoushi.h"
#include "IAICommand.h"
#include "Game.h"

Shiromadoushi::Shiromadoushi()
{
}
Shiromadoushi::~Shiromadoushi()
{
}
//白魔導士のAIはここに書く。
void Shiromadoushi::GenerateAICommandList(std::deque<IAICommandUPtr>& commands)
{
	m_commandsTmp.clear();
	AI(*senshi, *ryu, *shiro);
	//白魔導士に許されている行動は１ターン３回まで！
	for (int i = 0; i < 3 && i < m_commandsTmp.size(); i++) {
		commands.push_back(std::move(m_commandsTmp[i]));
	}
}