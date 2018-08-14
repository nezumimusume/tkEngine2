#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"
#include "tkEngine/physics/tkPhysicsGhostObject.h"
#include "tkEngine/character/tkCharacterController.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
private:
	void InitCamera();
	void InitSkinModelRender();
	void InitGhostObject();
private:
	prefab::CSkinModelRender* m_backGroundRender = nullptr;	//背景の描画処理。
	prefab::CSkinModelRender* m_playerRender = nullptr;		//プレイヤーの描画処理。
	CCharacterController m_charaCon;
	CPhysicsGhostObject m_ghostObject;						//ゴーストオブジェクトとを４つ。

	prefab::CPointLight* m_pointLig = nullptr;				//ポイントライト。
	prefab::CSoundSource* m_explosionSoundSource = nullptr;
};

