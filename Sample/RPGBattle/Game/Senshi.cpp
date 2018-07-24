#include "stdafx.h"
#include "Senshi.h"
#include "IAICommand.h"

Senshi::Senshi()
{
}


Senshi::~Senshi()
{
}
void Senshi::GenerateAICommandList(std::deque<IAICommandUPtr>& commands)
{
	//’§”­
	CommandChouhatu(commands, this);
}