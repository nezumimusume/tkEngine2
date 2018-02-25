#pragma once
class GameOverCamera;

class GameOverControl : public IGameObject
{
public:
	GameOverControl();
	~GameOverControl();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	GameOverCamera* m_gameOverCamera = nullptr;
};

