#pragma once

class Game;

/// <summary>
/// タイマーHUD
/// </summary>
class TimerHUD final : public IGameObject{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	TimerHUD();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TimerHUD();
	/// <summary>
	/// すたーちょ
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// あっぷでーと
	/// </summary>
	void Update() override;
private:
	prefab::CFontRender* m_fontRender = nullptr;	//フォント。
	Game* m_game = nullptr;
};

