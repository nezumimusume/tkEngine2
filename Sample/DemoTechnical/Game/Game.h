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
	prefab::CSkinModelRender* m_starModelRender = nullptr;		//星のモデルレンダラー。
	prefab::CSky* m_sky = nullptr;								//空。
	CShaderResourceView m_charaNormalMap;						//キャラの法線マップ。
	CShaderResourceView m_charaSpecMap;							//キャラのスペキュラマップ。
	CShaderResourceView m_groundSpecMap;						//地面のスペキュラマップ。
	prefab::CPointLight* m_starLight = nullptr;					//星のライト。
	CQuaternion m_charaRot = CQuaternion::Identity;
	CVector3 m_charaPos = CVector3::Zero;
	CQuaternion m_starRot = CQuaternion::Identity;
};

