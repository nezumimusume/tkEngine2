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
	void PostRender(CRenderContext& rc) override;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダラー。
	CVector3 pos = CVector3::Zero;
	CCharacterController m_charaCon;
	CPhysicsGhostObject m_meshGhost;
	CVector3 m_ghostPosition = CVector3::Zero;
	CQuaternion m_ghostRotation = CQuaternion::Identity;
	std::wstring m_fontText = L"Not hit";
	CFont m_font;
	float m_timer = 0.0f;
};

