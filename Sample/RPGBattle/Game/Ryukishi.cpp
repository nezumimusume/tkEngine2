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
	//“K“–‚É“G‚ğ‰£‚éB
	CommandAttackEnemy(commands, this);
}