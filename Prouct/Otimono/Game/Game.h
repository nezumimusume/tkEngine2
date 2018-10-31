#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"
#include "tkEngine/physics/tkPhysicsGhostObject.h"
#include "tkEngine/character/tkCharacterController.h"

class Game : public IGameObject
{
public:
	/// <summary>
	/// ゲームのステート。
	/// </summary>
	enum EnState {
		enState_Playing,	//プレイ中。
		enState_Timeup,		//タイムアップ。
	};
	Game();
	~Game();
	bool Start();
	void Update();
	/// <summary>
	/// プレイヤーが拾った星の数を取得する。
	/// </summary>
	/// <returns></returns>
	int GetStarCount() const
	{
		return m_getStarCount;
	}
	/// <summary>
	/// 星を取ったことを通知する。
	/// </summary>
	void NotifyGetStar()
	{
		m_getStarCount++;
	}
	/// <summary>
	/// ゲームの残り時間を取得を取得。
	/// </summary>
	/// <returns></returns>
	float GetRestTime() const
	{
		return m_restTimer;
	}
	/// <summary>
	/// ゲームのプレイ状態を取得。
	/// </summary>
	/// <returns></returns>
	EnState GetState() const
	{
		return m_state;
	}
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダラー。
	CLevel m_level;
	int m_getStarCount = 0;										//取得した星の数。
	prefab::CSoundSource* m_bgm = nullptr;						//BGM
	float m_restTimer = 5.0f;									//ゲームの残り時間。単位：秒。
	EnState m_state = enState_Playing;							//ゲームのステート。
	float m_clearEffectTimer = 0.0f;							//Clear演出タイム。
};

