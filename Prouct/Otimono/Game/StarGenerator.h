#pragma once
class StarGenerator : public IGameObject
{
public:
	StarGenerator();
	~StarGenerator();
	void Update() override;
private:
	float m_timer = 0.0f;	//タイマー(単位:秒)
};

