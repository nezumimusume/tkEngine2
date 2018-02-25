#pragma once
class GameClearCamera;
class Game;
class GameClearControl : public IGameObject
{
public:
	GameClearControl();
	~GameClearControl();
	
	void Awake() override;
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void PostRender(CRenderContext& rc);
private:
	bool m_isWaitFade = false;
	bool m_isEnd = false;
	GameClearCamera* m_gameClearCamera = nullptr;
	float m_timer = 0.0f;
	CFont m_font;
	Game* m_game = nullptr;
	CVector2 m_statScorePos = CVector2::Zero;
	float m_startScoreScale = 1.0f;
};

