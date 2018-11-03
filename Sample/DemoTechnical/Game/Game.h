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
	std::vector< prefab::CDirectionLight*> m_dirLights;			//平行光源のリスト
	prefab::CSkinModelRender* m_charaModelRender = nullptr;		//キャラクターのモデルレンダラー。
	prefab::CSkinModelRender* m_groundModelRender = nullptr;	//地面のモデルレンダラー。
	prefab::CSky* m_sky = nullptr;								//空。
	CShaderResourceView m_charaNormalMap;						//キャラの法線マップ。
	CShaderResourceView m_charaSpecMap;							//キャラのスペキュラマップ。
	CQuaternion m_charaRot = CQuaternion::Identity;
};

