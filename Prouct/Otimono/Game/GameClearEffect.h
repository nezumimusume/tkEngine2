#pragma once
/// <summary>
/// ゲームクリア演出。
/// </summary>
class GameClearEffect final : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	GameClearEffect();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GameClearEffect();
	/// <summary>
	/// 開始。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// アップデート。
	/// </summary>
	void Update() override;
};

