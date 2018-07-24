#pragma once

#include "AIAvator.h"
class Ryukishi : public AIAvator
{
public:
	Ryukishi();
	~Ryukishi();
	void GenerateAICommandList(std::deque<IAICommandUPtr>& commands) override;
};

