#pragma once

#include "AIAvator.h"

class Shiromadoushi : public AIAvator
{
public:
	Shiromadoushi();
	~Shiromadoushi();
	void GenerateAICommandList(std::deque<IAICommandUPtr>& commands) override;
	void AI(AIAvator& senshi, AIAvator& ryu, AIAvator& shiro);
private:
	std::deque<IAICommandUPtr> m_commandsTmp;
};

