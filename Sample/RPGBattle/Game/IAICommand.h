#pragma once

class AIAvator;

class IAICommand {
public:
	virtual ~IAICommand();
	
	virtual bool Execute() = 0;
protected:
	float m_timer = 0.0f;
};
using IAICommandUPtr = std::unique_ptr<IAICommand>;
//攻撃コマンド。
class AICommandAttack : public IAICommand {
public:	
	AICommandAttack(AIAvator* attackAvator, AIAvator* targetAvator) :
		m_attackAvator(attackAvator),
		m_targetAvator(targetAvator)
	{
	}
	bool Execute();
private:
	enum EnStep{
		enStep_PlayAttackAnimation,
		enStep_WaitText,
		enStep_WaitPlayEndAttackAnimation,
		enStep_WaitText2,
		enStep_WaitText3,
		enStep_WaitEnd,
	};
	EnStep m_step = enStep_PlayAttackAnimation;
	AIAvator* m_attackAvator = nullptr;		//攻撃を行うアバター。
	AIAvator* m_targetAvator = nullptr;		//攻撃を受けるアバター。
	float m_endTimer = 0.0f;
};

//挑発コマンド。
class AICommandChouhatu : public IAICommand {
public:
	AICommandChouhatu(AIAvator* avator) :
		m_avator(avator) {}
	bool Execute();
private:
	enum EnStep {
		enStep_Start,
		enStep_WaitMessage_00,
		enStep_WaitMessage_01
	};
	EnStep m_step = enStep_Start;
	AIAvator * m_avator = nullptr;
};
static inline void CommandAttack(std::deque<IAICommandUPtr>& commandList, AIAvator* attackAvator, AIAvator* targetAvator)
{
	IAICommandUPtr command = std::make_unique<AICommandAttack>(attackAvator, targetAvator);
	commandList.push_back(std::move(command));
}
//ホイミ。
class AICommandHoimi : public IAICommand {
public:
	AICommandHoimi(AIAvator* healAvator, AIAvator* targetAvator) :
		m_healAvator(healAvator),
		m_targetAvator(targetAvator) {}
	bool Execute() override;
private:
	enum EnStep {
		enStep_00,
		enStep_01,
		enStep_02,
	};
	AIAvator * m_healAvator = nullptr;
	AIAvator* m_targetAvator = nullptr;
	EnStep m_step = enStep_00;
};
//ザオリク。
class AICommandZaoriku : public IAICommand {
public:
	AICommandZaoriku(AIAvator* healAvator, AIAvator* targetAvator) :
		m_healAvator(healAvator),
		m_targetAvator(targetAvator) {}
	bool Execute() override;
private:
	enum EnStep {
		enStep_00,
		enStep_01,
		enStep_02,
	};
	AIAvator * m_healAvator = nullptr;
	AIAvator* m_targetAvator = nullptr;
	EnStep m_step = enStep_00;
};
//スクルト
class AICommandSukuruto : public IAICommand {
public:
	AICommandSukuruto(AIAvator* healAvator) :
		m_healAvator(healAvator) {}
	bool Execute() override;
private:
	enum EnStep {
		enStep_00,
		enStep_01,
		enStep_02,
	};
	AIAvator * m_healAvator = nullptr;
	AIAvator* m_targetAvator = nullptr;
	EnStep m_step = enStep_00;
};
//バイキルト
class AICommandBaikiruto : public IAICommand {
public:
	AICommandBaikiruto(AIAvator* healAvator, AIAvator* targetAvator) :
		m_healAvator(healAvator),
		m_targetAvator(targetAvator) {}
	bool Execute() override;
private:
	enum EnStep {
		enStep_00,
		enStep_01,
		enStep_02,
	};
	AIAvator * m_healAvator = nullptr;
	AIAvator* m_targetAvator = nullptr;
	EnStep m_step = enStep_00;
};
//天使の祈り
class AICommandTenshiNoInori : public IAICommand {
public:
	AICommandTenshiNoInori(AIAvator* healAvator) :
		m_healAvator(healAvator) {}
	bool Execute() override;
private:
	enum EnStep {
		enStep_00,
		enStep_01,
		enStep_02,
	};
	AIAvator * m_healAvator = nullptr;
	EnStep m_step = enStep_00;
};
void CommandAttackEnemy(std::deque<IAICommandUPtr>& commandList, AIAvator* attackAvator);
void CommandChouhatu(std::deque<IAICommandUPtr>& commandList, AIAvator* attackAvator);
void CommandHoim(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator);
void CommandZaoriku(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator);
void CommandSukuruto(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator);
void CommandBaikiruto(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator);
void CommandTenshiNoInori(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator);