#pragma once

#include "AIAvator.h"
class Senshi : public AIAvator
{
public:
	Senshi();
	~Senshi();
	void GenerateAICommandList(std::deque<IAICommandUPtr>& commands) override;
};

