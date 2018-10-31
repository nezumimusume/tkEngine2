#pragma once

class Game;

class ScoreHUD final : public IGameObject
{
public:
	ScoreHUD();
	~ScoreHUD();
	bool Start() override;
	void Update() override;
private:
	prefab::CFontRender* m_fontRender = nullptr;	//フォントレンダラー。
	Game* m_game = nullptr;							//ゲーム。
	float m_timer = 0.0f;
};

