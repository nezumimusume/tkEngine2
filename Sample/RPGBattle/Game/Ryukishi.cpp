#include "stdafx.h"
#include "Ryukishi.h"
#include "IAICommand.h"

Ryukishi::Ryukishi()
{
}
Ryukishi::~Ryukishi()
{
}
void Ryukishi::GenerateAICommandList(std::deque<IAICommandUPtr>& commands)
{
	//適当に敵を殴る。
	CommandAttackEnemy(commands, this);
}