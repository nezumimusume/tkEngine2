#pragma once

#include "tkEngine/sound/tkSoundSource.h"
#include "tkEngine/graphics/font/tkFont.h"

class Player;
class Background;
class GameCamera;
class Enemy;
class GameOverControl;
class Star;
class GameClearControl;
class StarRenderer;
class PlayerSilhouette;
class Fade;

class Game : public IGameObject
{
public:
	//メンバ関数。
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	
	void PostRender(CRenderContext& rc) override;
	void OnDestroy()override;
	void NotifyGameOver();
	void NotifyRestart();
	bool IsGameOver() const
	{
		return m_isGameOver;
	}
	bool IsGameClear() const
	{
		return m_isGameClear;
	}
	/*!
	 *@brief	敵を検索。
	 */
	Enemy* FindEnemy(std::function<bool(Enemy*)> findEnemyFunc)
	{
		Enemy* findEnemy = nullptr;
		auto findIt = std::find_if(m_enemyList.begin(), m_enemyList.end(), findEnemyFunc);
		if (findIt != m_enemyList.end()) {
			findEnemy = *findIt;
		}		
		return findEnemy;
	}
	void AddGetStarCount()
	{
		m_coinCount++;
	}
	DirectX::SpriteFont* GetFont() const
	{
		return m_timerFont.get();
	}
	const CVector2& GetScorePosition()
	{
		return m_scoreFontPosition;
	}
	void SetScorePosition(const CVector2& pos)
	{
		m_scoreFontPosition = pos;
	}
	float GetScoreFontScale()
	{
		return m_scoreFontScale;
	}
	void SetScoreFontScale( float scale )
	{
		m_scoreFontScale = scale;
	}
private:
	void InitSceneLight();
private:
	enum EnState {
		enState_FadeIn,	//!<フェードイン中。
		enState_InGame,	//!<インゲーム中。
	};
	Player*	m_player = nullptr;						//!<プレイヤー
	std::vector<Enemy*> m_enemyList;				//!<エネミーのリスト。
	Background* m_background = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameOverControl* m_gameOverControl = nullptr;	//!<ゲームオーバーコントロール。
	GameClearControl* m_gameClearControl = nullptr;	//!<ゲームクリアコントロール。
	StarRenderer* m_starRenderer = nullptr;
	std::vector<prefab::CPointLight*> m_pointLight;
	prefab::CDirectionLight* m_directionLight = nullptr;
	PlayerSilhouette* m_playerSilhouette = nullptr;
	bool m_isGameOver = false;
	bool m_isGameClear = false;
	prefab::CSoundSource* m_bgmSource = nullptr;		//!<BGM
	CFont m_fontTest;
	float m_timer = 30.0f;								//!<タイマー
	std::unique_ptr<DirectX::SpriteFont> m_timerFont;	//!<タイマー用のフォント。
	CVector2 m_scoreFontPosition;
	float m_scoreFontScale = 0.8f;
	int m_coinCount = 0;
	float m_waitTimer = 0.0f;
	bool m_isToneMapReset = false;
	EnState m_state = enState_FadeIn;
	Fade* m_fade = nullptr;
};

