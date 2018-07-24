#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	CVector3 pos = CVector3::Zero;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダラー。
	prefab::CSkinModelRender* m_bgSkinModelRender = nullptr;	//背景のスキンモデルレンダラー。
	std::vector<prefab::CDirectionLight*> m_directionLight;		//ディレクションライト。
	CShaderResourceView m_spec;
};

